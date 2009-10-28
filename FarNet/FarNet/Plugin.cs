/*
FarNet plugin for Far Manager
Copyright (c) 2005-2009 FarNet Team
*/

using System;
using System.IO;
using System.Security.Permissions;

namespace FarNet
{
	/// <summary>
	/// Base class of a preloadable plugin and not preloadable
	/// <see cref="ToolPlugin"/>, <see cref="CommandPlugin"/>, <see cref="EditorPlugin"/> and <see cref="FilerPlugin"/>.
	/// </summary>
	/// <remarks>
	/// It keeps reference to <see cref="IFar"/> and provides
	/// <see cref="Connect"/> and <see cref="Disconnect"/> methods.
	/// Normally a direct child should implement at least <see cref="Connect"/>.
	/// <para>
	/// Any direct child of this class is always preloadable and makes other plugins in the same assembly preloadable as well.
	/// </para>
	/// <para>
	/// If a plugin implements a single operation consider to use <see cref="ToolPlugin"/>, <see cref="CommandPlugin"/> or <see cref="FilerPlugin"/>.
	/// For a plugin that only installs editor events for specified file types consider <see cref="EditorPlugin"/>.
	/// These plugins are normally not preloadable and slightly easier to implement.
	/// </para>
	/// </remarks>
	public class BasePlugin
	{
		/// <summary>
		/// Protected constructor denies instances of this class.
		/// </summary>
		protected BasePlugin()
		{ }

		/// <summary>
		/// This object exposes FarNet API. It is set internally and should not be changed.
		/// </summary>
		public IFar Far { get; set; }

		/// <include file='doc.xml' path='docs/pp[@name="Connect"]/*'/>
		public virtual void Connect()
		{ }

		/// <summary>
		/// Override this to handle plugin shutdown.
		/// NOTE: don't call Far UI, it is not working if Far is exiting.
		/// </summary>
		[EnvironmentPermissionAttribute(SecurityAction.LinkDemand, Unrestricted = true)]
		public virtual void Disconnect()
		{ }

		/// <summary>
		/// Plugin or menu item name. By default it is the plugin class name.
		/// </summary>
		/// <remarks>
		/// If you override it (usually in <see cref="ToolPlugin"/>, <see cref="CommandPlugin"/> or <see cref="FilerPlugin"/>)
		/// and use several plugins in the assembly then make sure that names do not clash.
		/// </remarks>
		public virtual string Name
		{
			get
			{
				return GetType().FullName;
			}
		}

		/// <summary>
		/// Called before invoking of a command.
		/// A plugin may override it to perform some preparations.
		/// Example: PowerShellFar completes loading of the PowerShell engine.
		/// </summary>
		public virtual void Invoking()
		{ }
	}

	/// <summary>
	/// Plugin tool options.
	/// </summary>
	[Flags]
	public enum ToolOptions
	{
		/// <summary>
		/// None.
		/// </summary>
		None,
		/// <summary>
		/// Show the item in the config menu.
		/// </summary>
		Config = 1 << 0,
		/// <summary>
		/// Show the item in the disk menu.
		/// </summary>
		Disk = 1 << 1,
		/// <summary>
		/// Show the item in the menu called from the editor.
		/// </summary>
		Editor = 1 << 2,
		/// <summary>
		/// Show the item in the menu called from the panels.
		/// </summary>
		Panels = 1 << 3,
		/// <summary>
		/// Show the item in the menu called from the viewer.
		/// </summary>
		Viewer = 1 << 4,
		/// <summary>
		/// Show the item in the menu called from dialogs.
		/// </summary>
		Dialog = 1 << 5,
		/// <summary>
		/// Show the item in F11 menus.
		/// </summary>
		F11Menus = Panels | Editor | Viewer | Dialog,
		/// <summary>
		/// Show the item in F11 menus and in the disk menu.
		/// </summary>
		AllMenus = F11Menus | Disk,
		/// <summary>
		/// Show the item in F11 menus, the disk menu and the config menu.
		/// </summary>
		AllAreas = AllMenus | Config
	}

	/// <summary>
	/// Arguments of a tool plugin event.
	/// This event normally happens when a user selects a plugin menu item.
	/// </summary>
	public sealed class ToolEventArgs : EventArgs
	{
		///
		public ToolEventArgs(ToolOptions from)
		{
			_From = from;
		}
		/// <summary>
		/// Where it is called from.
		/// </summary>
		public ToolOptions From
		{
			get { return _From; }
		}
		ToolOptions _From;
		/// <summary>
		/// Tells to ignore results, e.g. when configuration dialog is cancelled.
		/// </summary>
		public bool Ignore { get; set; }
	}

	/// <summary>
	/// Base class of a FarNet tool represented by a single menu command in one or more Far menus.
	/// </summary>
	/// <remarks>
	/// It is enough to implement <see cref="Invoke"/> method only.
	/// Override other properties and methods as needed.
	/// You may derive any number of such classes.
	/// <include file='doc.xml' path='docs/pp[@name="InvokeLoad"]/*'/>
	/// </remarks>
	public abstract class ToolPlugin : BasePlugin
	{
		/// <summary>
		/// Tool handler.
		/// </summary>
		public abstract void Invoke(object sender, ToolEventArgs e);

		/// <summary>
		/// Tool options. By default the tool is shown in all menus.
		/// Override this to specify only really needed areas.
		/// </summary>
		public virtual ToolOptions Options
		{
			get { return ToolOptions.AllAreas; }
		}
	}

	/// <summary>
	/// Arguments of a command plugin event.
	/// </summary>
	public class CommandEventArgs : EventArgs
	{
		///
		public CommandEventArgs(string command)
		{
			_command = command;
		}
		string _command;
		/// <summary>
		/// Command to process.
		/// </summary>
		public string Command
		{
			get { return _command; }
		}
	}

	/// <summary>
	/// Base class of a FarNet plugin called from a command line by a command prefix.
	/// </summary>
	/// <remarks>
	/// You have to implement <see cref="Invoke"/> and provide <see cref="Prefix"/>.
	/// Override other properties and methods as needed.
	/// You may derive any number of such classes.
	/// <include file='doc.xml' path='docs/pp[@name="InvokeLoad"]/*'/>
	/// </remarks>
	public abstract class CommandPlugin : BasePlugin
	{
		/// <summary>
		/// Command handler.
		/// </summary>
		public abstract void Invoke(object sender, CommandEventArgs e);

		/// <summary>
		/// Command prefix. By default it is the class name, override <c>get</c> for another one.
		/// But it is only a suggestion, actual prefix may be changed by a user, so that
		/// if the plugin uses the prefix itself then override <c>set</c> too.
		/// </summary>
		public virtual string Prefix
		{
			get { return this.GetType().Name; }
			set { }
		}
	}

	/// <summary>
	/// Arguments for a handler registered by <see cref="IFar.RegisterFiler"/>.
	/// A handler is called to open a <see cref="IPluginPanel"/> which emulates a file system based on a file.
	/// If a file is unknown a handler should do nothing.
	/// </summary>
	public sealed class FilerEventArgs : EventArgs
	{
		///
		public FilerEventArgs(string name, Stream data, OperationModes mode)
		{
			_Name = name;
			_Data = data;
			_Mode = mode;
		}
		string _Name;
		/// <summary>
		/// Full name of a file including the path.
		/// If it is empty then a handler is called to create a new file [ShiftF1].
		/// In any case a handler opens <see cref="IPluginPanel"/> or ignores this call.
		/// </summary>
		public string Name
		{
			get { return _Name; }
		}
		Stream _Data;
		/// <summary>
		/// Data from the beginning of the file used to detect the file type.
		/// Use this stream in a handler only or copy the data for later use.
		/// </summary>
		public Stream Data
		{
			get { return _Data; }
		}
		OperationModes _Mode;
		/// <summary>
		/// Combination of the operation mode flags.
		/// </summary>
		public OperationModes Mode
		{
			get { return _Mode; }
		}
	}

	/// <summary>
	/// Base class of a FarNet file based plugin.
	/// </summary>
	/// <remarks>
	/// It is enough to implement <see cref="Invoke"/> method only.
	/// Override other properties and methods as needed.
	/// You may derive any number of such classes.
	/// <include file='doc.xml' path='docs/pp[@name="InvokeLoad"]/*'/>
	/// </remarks>
	public abstract class FilerPlugin : BasePlugin
	{
		/// <summary>
		/// Filer handler.
		/// </summary>
		public abstract void Invoke(object sender, FilerEventArgs e);

		/// <include file='doc.xml' path='docs/pp[@name="PluginFileMask"]/*'/>
		public virtual string Mask
		{
			get { return string.Empty; }
			set { }
		}

		/// <summary>
		/// Tells that the plugin also creates files.
		/// </summary>
		public virtual bool Creates
		{
			get { return false; }
			set { }
		}
	}

	/// <summary>
	/// Base class of a FarNet editor plugin.
	/// </summary>
	/// <remarks>
	/// It is enough to implement <see cref="Invoke"/> method only.
	/// Override other properties and methods as needed.
	/// You may derive any number of such classes.
	/// <include file='doc.xml' path='docs/pp[@name="InvokeLoad"]/*'/>
	/// </remarks>
	public abstract class EditorPlugin : BasePlugin
	{
		/// <summary>
		/// Editor <see cref="IAnyEditor.Opened"/> handler.
		/// </summary>
		public abstract void Invoke(object sender, EventArgs e);

		/// <include file='doc.xml' path='docs/pp[@name="PluginFileMask"]/*'/>
		public virtual string Mask
		{
			get { return string.Empty; }
			set { }
		}
	}

}
