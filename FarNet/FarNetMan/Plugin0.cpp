/*
FarNet plugin for Far Manager
Copyright (c) 2005 FarNet Team
*/

#include "StdAfx.h"
#include "Plugin0.h"
#include "Far.h"
#include "PluginInfo.h"

namespace FarNet
{;
void Plugin0::AddPlugin(BasePlugin^ plugin)
{
	if (!_plugins.Contains(plugin))
		_plugins.Add(plugin);
}

//! Don't use Far UI
void Plugin0::UnloadPlugin(BasePlugin^ plugin)
{
	LOG_AUTO(3, "Unload plugin " + plugin);

	_plugins.Remove(plugin);

	try
	{
		LOG_AUTO(3, String::Format("{0}.Disconnect", plugin));
		plugin->Disconnect();
	}
	catch(Exception^ e)
	{
		String^ msg = "ERROR: plugin " + plugin->ToString() + ":\n" + Log::FormatException(e) + "\n" + e->StackTrace;
		Console::ForegroundColor = ConsoleColor::Red;
		Console::WriteLine(msg);
		Log::TraceError(msg);

		System::Threading::Thread::Sleep(1000);
	}
}

//! Don't use Far UI
void Plugin0::UnloadPlugins()
{
	for(int i  = _plugins.Count; --i >= 0;)
		UnloadPlugin(_plugins[i]);

	_plugins.Clear();
}

void Plugin0::LoadPlugins()
{
	ReadCache();

	String^ path = Environment::ExpandEnvironmentVariables(ConfigurationManager::AppSettings["FarNet.Plugins"]);
	for each(String^ dir in Directory::GetDirectories(path))
	{
		// skip
		if (Path::GetFileName(dir)->StartsWith("-"))
			continue;

		// load
		LoadFromDirectory(dir);
	}
}

void Plugin0::LoadFromDirectory(String^ dir)
{
	try
	{
		// the only *.cfg
		array<String^>^ files = Directory::GetFiles(dir, "*.cfg");
		if (files->Length > 1)
			throw gcnew InvalidOperationException("More than one .cfg files found.");
		if (files->Length == 1)
		{
			LoadFromConfig(files[0], dir);
			return;
		}

		// DLLs
		for each(String^ dll in Directory::GetFiles(dir, "*.dll"))
			LoadFromAssembly(dll, nullptr);
	}
	catch(Exception^ e)
	{
		// Wish: no UI on loading
		String^ msg = "ERROR: plugin " + dir + ":\n" + Log::FormatException(e) + "\n" + e->StackTrace;
		Far::Instance->Write(msg, ConsoleColor::Red);
		Log::TraceError(msg);
	}
}

void Plugin0::LoadFromConfig(String^ file, String^ dir)
{
	for each(String^ line in File::ReadAllLines(file))
	{
		array<String^>^ classes = line->Split(gcnew array<Char>{' '}, StringSplitOptions::RemoveEmptyEntries);
		if (classes->Length == 0)
			continue;
		
		String^ assemblyName = classes[0];
		if (classes->Length == 1)
			throw gcnew InvalidDataException("Missed class list after '" + assemblyName + "' in the config file '" + file + "'.");

		LoadFromAssembly(Path::Combine(dir, assemblyName), classes);
	}
}

void Plugin0::LoadFromAssembly(String^ assemblyPath, array<String^>^ classes)
{
	// loaded from cache?
	String^ dllName = Path::GetFileName(assemblyPath);
	if (_cache.ContainsKey(dllName))
		return;

	// load from assembly
	int nBasePlugin = 0;
	List<CommandPluginInfo^> commands;
	List<EditorPluginInfo^> editors;
	List<FilerPluginInfo^> filers;
	List<ToolPluginInfo^> tools;
	Assembly^ assembly = Assembly::LoadFrom(assemblyPath);
	if (classes)
	{
		for(int i = 1; i < classes->Length; ++i)
			nBasePlugin += AddPlugin(assembly->GetType(classes[i], true), %commands, %editors, %filers, %tools);
	}
	else
	{
		int nLoaded = 0;
		for each(Type^ type in assembly->GetExportedTypes())
		{
			if (type->IsAbstract)
				continue;
			if (BasePlugin::typeid->IsAssignableFrom(type))
			{
				++nLoaded;
				nBasePlugin += AddPlugin(type, %commands, %editors, %filers, %tools);
			}
		}

		if (nLoaded == 0)
			throw gcnew InvalidOperationException("Assembly '" + assemblyPath + "' has no valid BasePlugin derived classes. Remove this assembly from the directory or use a .cfg file.");
	}

	// add plugins
	if (commands.Count)
		Far::Instance->RegisterCommands(%commands);
	if (editors.Count)
		Far::Instance->RegisterEditors(%editors);
	if (filers.Count)
		Far::Instance->RegisterFilers(%filers);
	if (tools.Count)
		Far::Instance->RegisterTools(%tools);

	// write cache
	if (nBasePlugin == 0)
		WriteCache(assemblyPath, %commands, %editors, %filers, %tools);
}

int Plugin0::AddPlugin(Type^ type, List<CommandPluginInfo^>^ commands, List<EditorPluginInfo^>^ editors, List<FilerPluginInfo^>^ filers, List<ToolPluginInfo^>^ tools)
{
	// create
	BasePlugin^ instance = (BasePlugin^)Activator::CreateInstance(type);

	LOG_AUTO(3, "Load plugin " + instance);

	// register, attach connect
	_plugins.Add(instance);
	instance->Far = Far::Instance;
	{
		LOG_AUTO(3, String::Format("{0}.Connect", instance));
		instance->Connect();
	}

	// case: tool
	ToolPlugin^ tool = dynamic_cast<ToolPlugin^>(instance);
	if (tool)
	{
		ToolPluginInfo^ pt = gcnew ToolPluginInfo(tool, tool->Name, gcnew EventHandler<ToolEventArgs^>(tool, &ToolPlugin::Invoke), tool->Options);
		tools->Add(pt);
		return 0;
	}

	// case: command
	CommandPlugin^ command = dynamic_cast<CommandPlugin^>(instance);
	if (command)
	{
		CommandPluginInfo^ pc = gcnew CommandPluginInfo(command, command->Name, command->Prefix, gcnew EventHandler<CommandEventArgs^>(command, &CommandPlugin::Invoke));
		command->Prefix = pc->Prefix;
		commands->Add(pc);
		return 0;
	}

	// case: editor
	EditorPlugin^ editor = dynamic_cast<EditorPlugin^>(instance);
	if (editor)
	{
		EditorPluginInfo^ pe = gcnew EditorPluginInfo(editor, editor->Name, gcnew EventHandler(editor, &EditorPlugin::Invoke), editor->Mask);
		editor->Mask = pe->Mask;
		editors->Add(pe);
		return 0;
	}

	// case: filer
	FilerPlugin^ filer = dynamic_cast<FilerPlugin^>(instance);
	if (filer)
	{
		FilerPluginInfo^ pf = gcnew FilerPluginInfo(filer, filer->Name, gcnew EventHandler<FilerEventArgs^>(filer, &FilerPlugin::Invoke), filer->Mask, filer->Creates);
		filer->Mask = pf->Mask;
		filers->Add(pf);
		return 0;
	}

	return 1;
}

void Plugin0::ReadCache()
{
	RegistryKey^ keyCache;
	try
	{
		keyCache = Registry::CurrentUser->CreateSubKey(Far::Instance->RootKey + "\\FarNet\\<cache>");
		for each (String^ dllName in keyCache->GetSubKeyNames())
		{
			bool ok = true;
			RegistryKey^ keyDll;
			RegistryKey^ keyPlugin;
			List<CommandPluginInfo^> commands;
			List<EditorPluginInfo^> editors;
			List<FilerPluginInfo^> filers;
			List<ToolPluginInfo^> tools;
			try
			{
				keyDll = keyCache->OpenSubKey(dllName);

				String^ assemblyPath = keyDll->GetValue("Path", String::Empty)->ToString();
				if (!assemblyPath->Length || !File::Exists(assemblyPath))
					throw gcnew OperationCanceledException;

				String^ assemblyStamp = keyDll->GetValue("Stamp", String::Empty)->ToString();
				FileInfo fi(assemblyPath);
				if (assemblyStamp != fi.LastWriteTime.Ticks.ToString(CultureInfo::InvariantCulture))
				{
					// do not throw, it's PITA in debugging
					ok = false;
				}
				else
				{
					for each (String^ className in keyDll->GetSubKeyNames())
					{
						keyPlugin = keyDll->OpenSubKey(className);

						String^ pluginName = keyPlugin->GetValue("Name", String::Empty)->ToString();
						if (!pluginName->Length)
							throw gcnew OperationCanceledException;

						String^ type = keyPlugin->GetValue("Type", String::Empty)->ToString();
						if (type == "Tool")
						{
							int options = (int)keyPlugin->GetValue("Options");
							if (!options)
								throw gcnew OperationCanceledException;

							ToolPluginInfo^ plugin = gcnew ToolPluginInfo(assemblyPath, className, pluginName, (ToolOptions)options);
							tools.Add(plugin);
						}
						else if (type == "Command")
						{
							String^ prefix = keyPlugin->GetValue("Prefix", String::Empty)->ToString();
							if (!prefix->Length)
								throw gcnew OperationCanceledException;

							CommandPluginInfo^ plugin = gcnew CommandPluginInfo(assemblyPath, className, pluginName, prefix);
							commands.Add(plugin);
						}
						else if (type == "Editor")
						{
							String^ mask = keyPlugin->GetValue("Mask", String::Empty)->ToString();

							EditorPluginInfo^ plugin = gcnew EditorPluginInfo(assemblyPath, className, pluginName, mask);
							editors.Add(plugin);
						}
						else if (type == "Filer")
						{
							String^ mask = keyPlugin->GetValue("Mask", String::Empty)->ToString();
							int creates = (int)keyPlugin->GetValue("Creates", (Object^)-1);

							FilerPluginInfo^ plugin = gcnew FilerPluginInfo(assemblyPath, className, pluginName, mask, creates != 0);
							filers.Add(plugin);
						}
						else
						{
							throw gcnew OperationCanceledException;
						}

						keyPlugin->Close();
					}

					keyDll->Close();

					// add dllName to dictionary and add plugins
					_cache.Add(dllName, nullptr);
					if (commands.Count)
						Far::Instance->RegisterCommands(%commands);
					if (editors.Count)
						Far::Instance->RegisterEditors(%editors);
					if (filers.Count)
						Far::Instance->RegisterFilers(%filers);
					if (tools.Count)
						Far::Instance->RegisterTools(%tools);
				}
			}
			catch(OperationCanceledException^)
			{
				ok = false;
			}
			catch(Exception^ ex)
			{
				throw gcnew OperationCanceledException(
					"Error on reading the cache. Remove registry FarNet\\<cache> manually and restart Far.", ex);
			}

			// error or outdated info
			if (!ok)
			{
				if (keyPlugin)
					keyPlugin->Close();
				if (keyDll)
					keyDll->Close();
				keyCache->DeleteSubKeyTree(dllName);
			}
		}
	}
	finally
	{
		if (keyCache)
			keyCache->Close();
	}
}

void Plugin0::WriteCache(String^ assemblyPath, List<CommandPluginInfo^>^ commands, List<EditorPluginInfo^>^ editors, List<FilerPluginInfo^>^ filers, List<ToolPluginInfo^>^ tools)
{
	FileInfo fi(assemblyPath);
	RegistryKey^ keyDll;
	try
	{
		keyDll = Registry::CurrentUser->CreateSubKey(Far::Instance->RootKey + "\\FarNet\\<cache>\\" + fi.Name);
		keyDll->SetValue("Path", assemblyPath);
		keyDll->SetValue("Stamp", fi.LastWriteTime.Ticks.ToString(CultureInfo::InvariantCulture));

		for each(ToolPluginInfo^ plugin in tools)
		{
			RegistryKey^ key = keyDll->CreateSubKey(plugin->ClassName);
			key->SetValue("Type", "Tool");
			key->SetValue("Name", plugin->Name);
			key->SetValue("Options", (int)plugin->Options);
			key->Close();
		}

		for each(CommandPluginInfo^ plugin in commands)
		{
			RegistryKey^ key = keyDll->CreateSubKey(plugin->ClassName);
			key->SetValue("Type", "Command");
			key->SetValue("Name", plugin->Name);
			key->SetValue("Prefix", plugin->DefaultPrefix);
			key->Close();
		}

		for each(EditorPluginInfo^ plugin in editors)
		{
			RegistryKey^ key = keyDll->CreateSubKey(plugin->ClassName);
			key->SetValue("Type", "Editor");
			key->SetValue("Name", plugin->Name);
			key->SetValue("Mask", plugin->DefaultMask);
			key->Close();
		}

		for each(FilerPluginInfo^ plugin in filers)
		{
			RegistryKey^ key = keyDll->CreateSubKey(plugin->ClassName);
			key->SetValue("Type", "Filer");
			key->SetValue("Name", plugin->Name);
			key->SetValue("Mask", plugin->DefaultMask);
			key->SetValue("Creates", (int)plugin->Creates);
			key->Close();
		}
	}
	finally
	{
		if (keyDll)
			keyDll->Close();
	}
}

}
