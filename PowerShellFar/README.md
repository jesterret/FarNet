[Contents]: #powershellfar
[FAQ]: #frequently-asked-questions
[Examples]: #command-and-macro-examples

# PowerShellFar

FarNet module for Far Manager

* [About](#about)
* [Installation](#installation)
* [Run commands](#run-commands)
* [Command line](#command-line)
* [Menu commands](#menu-commands)
* [Interactive](#interactive)
* [Power panel](#power-panel)
* [Folder tree](#folder-tree)
* [Data panel](#data-panel)
* [Tree panel](#tree-panel)
* [Global objects](#global-objects)
* [Profiles](#profiles)
* [Settings](#settings)

**Details**

* [Commands output](#commands-output)
* [Background jobs](#background-jobs)
* [Debugger dialog](#debugger-dialog)
* [Frequently asked questions][FAQ]
* [Command and macro examples][Examples]

**Scripts**

* [Suffixes](#suffixes)
* [Profile.ps1](#profileps1)
* [Profile-Editor.ps1](#profile-editorps1)
* [TabExpansion2.ps1](#tabexpansion2ps1)
* [Remove-EndSpace-.ps1](#remove-endspace-ps1)
* [Job-RemoveItem-.ps1](#job-removeitem-ps1)
* [Search-Regex-.ps1](#search-regex-ps1)
* [Other scripts](#other-scripts)

*********************************************************************
## About

[Contents]

PowerShellFar is the FarNet module for Far Manager, the file manager.
It is the Windows PowerShell host in the genuine console environment.

PowerShellFar exposes the FarNet API and provides many ways for invoking
commands and viewing the results. The package includes cmdlets, modules,
and scripts designed for Far Manager. Syntax highlighting in the editor
is provided by Colorer.

**Project FarNet**

* Wiki: <https://github.com/nightroman/FarNet/wiki>
* Site: <https://github.com/nightroman/FarNet>
* Author: Roman Kuzmin

*********************************************************************
## Installation

[Contents]

**Requirements**

- Windows PowerShell 2.0+
- Far Manager, see the required version in *History.txt*
- Plugin FarNet, see the required version in *History.txt*

**Instructions**

In order to install FarNet and its modules, follow these steps:

<https://raw.githubusercontent.com/nightroman/FarNet/master/Install-FarNet.en.txt>

---
**Set PowerShell execution policy**

The module does not require this, it invokes scripts with any policy.
This step is recommended for invoking scripts by *PowerShell.exe*.

Start `PowerShell.exe` as administrator, and type `Get-ExecutionPolicy`. If it
is not *Bypass*, *Unrestricted*, or *RemoteSigned* invoke `Set-ExecutionPolicy`
with the required value. *Bypass* is the least secure but it may work faster.

On x64 machines `Set-ExecutionPolicy` should be set for x86 and x64. Use the
Windows start menu in order to open x86 and x64 consoles and set the policy.

If you are not administrator use the parameter `-Scope CurrentUser`.

---
**Documentation**

These files are not installed but included to the package:

- *About-PowerShellFar.htm* - this documentation
- *History.txt* - the change log

---
**Bench scripts**

Included Bench scripts are ready to use tools for various tasks. There are also
test and demo scripts for learning PowerShellFar and PowerShell. In order to to
use these scripts directly from Bench include the directory Bench in the path.

New users may want to configure the module with [Profile.ps1](#profileps1). Then
it should be copied to *%FARPROFILE%\FarNet\PowerShellFar*. It adds various
features including additional menu items invoking the scripts.

**Colorer support**

The syntax scheme *powershell.hrc* and the optional color scheme *visual.hrd*
come with the Colorer package. They provide syntax highlighting in PowerShell
files.

**Console window**

If you use a lot of PowerShell commands with console output consider to start
Far Manager in window mode (`Far.exe /w`) and set/save the large enough buffer
height. Alternatively, set this mode permanently (`System.WindowMode = true`)
in the list of settings (type `far:config` in the command line).

If you work a lot with power panels then set/save the console width at least
164, so that panel widths are 80 or more. Many PowerShell type formats are
designed with 80 columns in mind.


*********************************************************************
## Run commands

[Contents]

There are many ways of invoking PowerShell commands in Far Manager, you can
type and invoke commands at any moment in almost any current context.

**Command line**

Use the Far Manager command line to type and invoke commands with the prefixes
`ps:` and `vps:`, see [Command line](#command-line).

**Invoke commands**

The input box can be opened at any moment: `[F11] \ PowerShellFar \ Invoke
commands`. See [Invoke commands dialog](#invoke-commands-dialog).

**Selected code**

Selected text in any editor including the command line and dialog edit boxes
can be invoked as PowerShell code: `[F11] \ PowerShellFar \ Invoke selected`.

**Interactive**

Main, local, remote editor interactive: `[F11] \ PowerShellFar \ Interactive`.
Local and remote editors use new sessions and invoke commands asynchronously,
UI is blocked when commands are running, even with output in progress. See
[Interactive](#interactive).

**Script editor**

A script opened in the editor can be invoked by `[F5]` in the editor. It is the
same as to invoke the script without parameters from the command line or the
command box.

**Far Manager macros**

Macros associate key combinations in UI areas with pieces of macro code which
invoke PowerShell commands. See [Examples].

**User menu and file associations**

The user menu (`[F2]`) and file associations (`[F9] \ Commands \ File
associations`) may include PowerShell commands with prefixes. See Far Manager
help for details. Note that the user menu can be opened in any area, not just
panels, but Far Manager does not provide a standard key, so choose a key and
assign a macro `mf.usermenu(0, "")`.

**Event handlers**

Various event handlers can be added using the profiles.
See [Profile.ps1](#profileps1) and [Profile-Editor.ps1](#profile-editorps1).

---
**Stopping running commands**

`[CtrlBreak]` stops synchronous commands invoked from:

- Command line
- Command input box
- Main interactive
- File associations
- User menu (`[F2]`)
- Script editor (`[F5]`)

`[CtrlC]` stops asynchronous commands commands invoked from:

- Local interactive
- Remote interactive

`[Del]` in the background job list stops running background jobs.

It is not normally possible to stop commands started from event handlers like
editor and panel events. Design such code carefully and test it well before
adding to handlers.


*********************************************************************
## Command line

[Contents]

Commands with prefixes are used in the command line, the user menu (`[F2]`),
and the file associations. By default command prefixes are: `ps:` (console
output) and `vps:` (viewer output).

Console output may be transcribed to a file, use `Start-Transcript` and
`Stop-Transcript` for starting and stopping and `Show-FarTranscript` for
viewing the output.

Note: PowerShell scripts opened in the editor can be invoked in the current
session by `[F5]`. The key is hard-coded but a different key can be used, too.

See [Examples].

Commands with console output, prefix `ps:`

    ps: Get-Date
    ps: 3.14 / 3
    ps: [math]::pi / 3

Commands with viewer output, prefix `vps:`

    vps: Get-Process
    vps: Get-ChildItem C:\TEMP\LargeFolder -Recurse -Force

Commands starting UI or background jobs, normally prefix `ps:`

    ps: $Far.Message("Hello world")
    ps: Get-Process | Out-FarList -Text Name | Open-FarPanel
    ps: Start-FarJob { Remove-Item C:\TEMP\LargeFolder -Recurse -Force }

**Accelerators**

You can reduce typing by "Easy prefix" and "Easy invoke" macros, see [FAQ].

**Command history**

Most of commands are added to the PowerShellFar command history automatically.
A command is not added if it ends with "#". This may be useful for commands in
the user menu (`[F2]`) and the file associations. Such commands are not typed
manually and normally should not pollute the history.


*********************************************************************
## Menu commands

[Contents]

**Invoke commands**

You are prompted to enter PowerShell commands to be invoked. Output is shown in
a viewer. See [Invoke commands dialog](#invoke-commands-dialog) for more details.

**Invoke selected**

In editor: execute the selected code or the current line. Thus, you can try
your code just in the editor. The code is executed in the global scope,
variables created by commands are available for other commands.

In command line: execute the command line text, the prefix is not required but
it is not an error. The command text is kept in the command line if an error
happens or if there is a selection.
See also "Easy invoke macro": [FAQ].

**Background jobs**

Shows the background jobs menu.
See [Background jobs menu](#background-jobs-menu).

**Command history**

Shows the command history list.
See [Command history](#command-history).

**Interactive**

Opens a new interactive file associated with its main, local, or remote session.
Files are stored in *%FARLOCALPROFILE%\FarNet\PowerShellFar*.
Several interactive editors are allowed.
See [Interactive](#interactive).

**Power panel**

Shows a menu with a list of panels available for opening.
See [Power panel menu](#power-panel-menu) and [Power panel](#power-panel).

**Complete**

It calls TabExpansion for the text in the editor, the command line, or an edit
box in a dialog. The built-in PowerShell TabExpansion is replaced with advanced
[TabExpansion2.ps1](#tabexpansion2ps1).

**Errors**

Shows the recent PowerShell errors stored in the global variable `$Error`.
See [Errors menu](#errors-menu).

**Help**

For the current token in an editor, an edit box, or the command line it shows
available help information in the viewer. In code editors (*.ps1*, *.psm1*,
*.psd1*, input code boxes) this action is associated with `[ShiftF1]`. It is
exposed for scripting as `$Psf.ShowHelp()`.


*********************************************************************
## Invoke commands dialog

[Contents]
[Menu commands](#menu-commands)

The input box can be opened at any moment:
`[F11] \ PowerShellFar \ Invoke commands`.

This dialog is used for typing and invoking PowerShell commands.
The output is shown in the viewer.

**Keys and actions:**

<!--_140305_113849-->

- `[Enter]`

    Invokes the typed commands.

- `[Tab]`

    Invokes PowerShell code completion (TabExpansion).

- `[F1]`

    If the input line is empty shows this topic. Otherwise shows PowerShell
    help for the current command or its parameter.

<!--_140305_113849-->


*********************************************************************
## Background jobs menu

[Contents]
[Background jobs](#background-jobs)

Shows the background jobs information. Job states:

- Running (`[Del]` stops a job)
- Stopped (e.g. by `[Del]`)
- Errors (there is one or more not terminating errors)
- Failed (there is a terminating error)
- Completed (there are no errors)

Other information: output data size and a job name or its command text (shortened).

**Keys and actions**

- `[Enter]`

    If there are output data closes the menu and opens a viewer for the output.

- `[F3]`

    The same but you will return to the job menu when you close a viewer.

- `[F5]`

    Refreshes job data shown by this menu.

- `[Del]`

    For a running job, stops it. For a not running job, removes it from the
    list, discards its output, errors, and deletes temp files, if any.

- `[ShiftDel]`

    Invokes `[Del]` action for each job in the list.


*********************************************************************
## Command history

[Contents]

The command history is shown by `[F11] \ PowerShellFar \ Command history`.

**Keys and actions**

- `[Enter]`

    Inserts a command to the command input line (panels, interactive, command
    box) or shows a new dialog *Invoke commands* with this command inserted.

- `[CtrlC]`

    Copies the command text to the clipboard.

- `[CtrlR]`, `[Del]`

    Removes old and duplicated commands from the history.

---
**Incremental filter**

Typed characters are immediately applied as the filter. `*` and `?` are treated
as wildcard characters.

- `[BS]`

    Removes the last character from the incremental filter.

- `[ShiftBS]`

    Removes the incremental filter string completely.


*********************************************************************
## Power panel menu

[Contents]
[Power panel](#power-panel)

This menu shows available special panels and PowerShell provider drives for
opening provider panels. `[Enter]` opens the selected panel.


*********************************************************************
## Errors menu

[Contents]

This menu shows the list of recent PowerShell errors, i.e. ErrorRecord objects
stored in the global variable $Error. Menu items of error objects with source
info are checked on the left.

**Keys and actions**

- `[F4]`

    If source information is available (checked items) it opens a source file
    in the internal editor at the line that caused an error.

- `[Enter]`

    Shows the FarNet error message dialog for the selected item.

- `[Del]`

    Removes all error records and closes the menu.


*********************************************************************
## Debugger menu

[Contents]

This menu consists of two sections: commands to create various breakpoints and
the list of existing breakpoints.

**Create breakpoint actions**

This section contains commands that create various breakpoints. There are
three kind of breakpoints in PowerShell: line, command and variable.
See also [Breakpoint dialog](#breakpoint-dialog).

* Line breakpoint...

    Opens a dialog to create a new line breakpoint. If the
    command is invoked from an editor and a line breakpoint already exists at the
    current editor line then you are prompted to remove, enable/disable, modify the
    existing breakpoint or add a new one at the same line.

* Command breakpoint...

    Opens a dialog to create a new command breakpoint.

* Variable breakpoint...

    Opens a dialog to create a new variable breakpoint.

---
**Breakpoint list keys and actions**

This section shows the list of available breakpoints where you can disable, enable
or remove breakpoints.

* `[F4]`

    Opens the source script in the editor for the current line breakpoint or
    another kind of breakpoint with a script.

* `[Space]`

    Enables or disables the current breakpoint.

* `[ShiftBS]`

    Disables all breakpoints.

* `[Del]`

    Removes the current breakpoint.

* `[ShiftDel]`

    Removes all existing breakpoints.


*********************************************************************
## Breakpoint dialog

[Contents]

The dialog creates a new breakpoint. There are three kind of breakpoints in
PowerShell:

* Line breakpoint

    You have to provide a script line number, script file
    path (mandatory) and optional action code.

* Command breakpoint

    You have to provide a command name (mandatory),
    optional script path and optional action code.

* Variable breakpoint

    You have to provide a variable name (mandatory),
    optional script path and optional action code.

Script path is mandatory only for line breakpoints. But you can specify it for
other breakpoints, too; in this case breakpoint scope is limited to the script.
If you open a breakpoint dialog from the editor then the path of the file being
edited is inserted by default. Clean or change it if it is not what you want.

If you do not provide action code then breakpoints are created with standard
action - breaking into debugger. Otherwise breakpoint action completely depends
on the code. You can do a lot of useful things using breakpoints with actions:
debugging, logging, diagnostics, adding extra or altering original features
without changing the original source code - all depends on your fantasy.


*********************************************************************
## Debugger dialog

[Contents]

This dialog is shown when one or more breakpoints are hit or you step through
the code in the debugger. You may step through the code, open interactive,
take a look at the source code, or stop the pipeline and debugging.

**Keys, buttons, and actions**

- `[Esc]`, `[F10]`

    (Continue) Continues execution.

- `{Step}`

    (Step Into) Executes the next statement. It steps through the script one
    line at a time.

- `{Over}`

    (Step Over) Executes the next statement, but skips over statements in
    functions or other scripts. The functions and scripts are executed, but it
    does not stop at each statement.

- `{Out}`

    (Step Out) Runs the script until completion or the next breakpoint. If used
    while stepping through a function, it exits the function and steps to the
    next statement.

- `{Interactive}`

    Opens the interactive. There you can examine or change variables, invoke
    commands, and etc. On exit the debugger dialog is repeated.

- `{Edit}`

    Opens the source in the editor at the debugger line. The editor is locked
    because changes during debugging normally should be avoided. Of course the
    editor can be unlocked and the script modified but normal debugging may be
    difficult after that. On exit the debugger dialog is repeated.

- `{View}`

    Opens an external viewer to view commands output. This action is available
    for: 1) commands with viewer output; 2) commands with console output if
    `Start-Transcript` was called. It is not available for interactive.

- `{Line}`

    Sets the current list line to the current debugger line.

- `{Quit}`

    Stops execution and exits the debugger.

---
**Notes**

It is possible that the debugger or your actions there may affect execution
flow in unusual way, especially if the code deals with Far UI that may clash
against the debugger UI. Think when you are about to debug such scenarios.

On debugging commands with console output it is useful to `Start-Transcript`
before debugging. In this case the output can be shown by the button `{View}`
or by the command `Show-FarTranscript`.


*********************************************************************
## Interactive

[Contents]
[Console applications](#console-applications)

Interactive is a *.interactive.ps1* file opened in the editor. Such a file is
opened in the special console-like mode designed for typing and invoking
PowerShell commands with their output appended to the end of editor text.

A new interactive is normally opened from the *Interactive* menu with three
choices: *Main session*, *New local session*, and *New remote session*.

An interactive is also opened from the debugger dialog and on entering a
nested prompt, for example when PowerShell execution is suspended.

An interactive works similar to a traditional command console but it is
still an editor window with some rules and special hotkeys.

**Command and output areas**

Output areas are lines between markers `<#<` and `>#>`.
Empty output may be represented as `<##>`.
Areas between output areas are commands.
The last area is the active command.

Output can be edited as usual text. Note that caress modification of marker
lines may confuse the interactive tools, syntax highlighting, and etc.

**Keys and actions**

Without selection some editor events are special:

* `[ShiftEnter]`

    If the caret is in the active command then the command is invoked and its
    output is appended to the end marked by `<#<` and `>#>` or `<##>` .

    If it is in the passive command then its code is appended to the active
    command and the caret is moved there as well.

* `[Tab]`

    Invokes PowerShell TabExpansion.
    See [TabExpansion2.ps1](#tabexpansion2ps1)

* `[F6]`

    Opens the interactive history list menu.
    The selected text is appended to the end.

* `[ShiftDel]`

    Replaces the nearest upper output `<#<...>#>` with `<##>`.

* `[CtrlBreak]`

    Stops running synchronous commands in the global console.

* `[CtrlC]`

    Stops running asynchronous commands in the local or remote console.

---

Interactive editors may be more convenient than consoles. But they have
limitations. Some of them are described below.

**Any interactive**

Native console applications with user interaction should not be called.

---
**Main sessions (synchronous)**

Do not invoke commands with `$Far.Editor` (i.e. this editor) because during the
operation this object is already used for the command output.

---
**Local and remote sessions (asynchronous)**

Each interactive opens a separate runspace with its private session state:
provider locations, variables, functions, aliases, and etc.

Commands are invoked asynchronously in background threads, so that console
editors and Far itself are not blocked: you can switch to panels or another
editors while a command is running. Moreover, you can open several async
consoles, invoke parallel commands and still continue other work in Far.

Limitations of asynchronous consoles:

- Objects `$Far` and `$Psf` are not exposed.
- Cmdlets `*-Far*` are exposed but normally should not be used.
- PowerShell UI should be avoided: `Read-Host`, `Write-Progress`, confirmations, ...

---
**Notes**

The *Colorer* scheme *powershell.hrc* (optionally with *visual.hrd*) takes care
of syntax highlighting. In addition to PowerShell syntax console output is
colored as well: numbers, dates, times, paths, errors, warnings, and etc.

Use word completion (e.g. *Complete-Word-.ps1*): words from output of previous
commands often can be effectively completed in a new command being composed.


*********************************************************************
## Interactive menu

[Contents]
[Interactive](#interactive)

The menu opens a new interactive for the current main, or a new local or remote
session.

* Main session

    Opens a console for the main PowerShell session. Commands are invoked
    synchronously in the default runspace as if they invoked from the command
    line. Output is sent to the same editor. All main consoles share the same
    workspace.

* New local session

    Opens a console for a new local PowerShell session. Commands are invoked
    asynchronously in a new runspace. When the console closes all session data
    are removed.

* New remote session

    Opens a console in a new remote PowerShell session. Commands are invoked
    asynchronously in a new remote runspace. You are prompted to enter a
    computer name and a user domain\name. If a user is specified then a
    password is also requested.


*********************************************************************
## Power panel

[Contents]

Power panel is a PowerShellFar panel with .NET objects, PowerShell provider
items, object or item properties and etc. There are several panels:

* [Object panel](#object-panel)

    Table of any .NET objects, normally of the same type or the same base type.
    Columns (default or custom) show property values. The simplest way to use a
    panel is `Out-FarPanel` cmdlet.

* [Member panel](#list-panel)

    List of members (properties, methods, and etc.) of
    a .NET object. There are two columns: Name and Value.

* [Provider item panel](#item-panel)

    Table of PowerShell provider items in a
    specified path. Columns (default or custom) show item properties.

* [Provider folder tree](#folder-tree)

    Tree of PowerShell provider container
    items. Providers that support container items: FileSystem, Registry, ...

* [Provider property panel](#list-panel)

    List of provider properties of an item.
    Providers that support them: FileSystem, Registry, ...

**Keys and actions**

Availability and details of operations may depend on a panel type, mode, data
types, providers and etc.

* `[F1]`

    Opens the panel help menu with available panel specific commands. `[F1]`
    pressed in this menu opens the panel help topic.

* `[F3]`

    Views content, properties or other PowerShell or .NET information about the
    current object, provider item, member, property or '..' element.

* `[F4]`

    Starts internal editor for editing the current item content or a property
    value. Note: some items or properties are read only or cannot be assigned
    in this way - this is not always recognized on opening. Editor is not
    modal, you can edit other items at the same time. Data are updated on
    saving. It is recommended to save only by `[F2]` (i.e. not on exiting) - in
    this case on errors you are still in editor and can fix problems.

* `[AltF4]`

    Starts Notepad. In contrast to `[F4]` you have to finish editing and exit.
    If there are errors then Notepad is started again with the same temp file,
    i.e. your changes are not lost, you can try to fix them.

* `[F5]`

    Copies items or properties from the active panel to another. You can copy
    almost any items to an Object panel, so that an Object panel can be used as
    a collector of items for further operations.

* `[ShiftF5]`

    Copies the current provider item, dynamic property, and etc. here with
    another name.

* `[F6]`

    Moves items or properties to another panel. You can move items to Object
    panel but it works the same as copying (`[F5]`).

* `[ShiftF6]`

    Renames the current provider item, dynamic property, and etc.

* `[F7]`

    Creates a new item or a property or invokes similar actions. Depending on a
    provider you may have to specify required provider item or property type or
    initial value. (If you don't know what to enter then enter something and
    follow error message instructions (or read provider manuals)). In Object
    mode an empty object is created for you and you are prompted to create the
    first property (so called NoteProperty).

* `[F8]`, `[Del]`

    Removes selected objects from the panel and, depending on a panel, performs
    actions on related system objects. Object panel: removes objects from the
    panel. Item panels: deletes the selected items or the current item or
    dynamic properties. Confirmations depend on Far settings for delete
    operations, but confirmation dialog is not exactly the same as in Far.

* `[Esc]`

    Closes or clears the panel. You may be prompted to clarify.

* `[ShiftEsc]`

    Closes the current panel together with all its parent panels.

* `[Enter]`

    Enters folders, opens items (registered file types, etc.), etc. Actual
    action depends on a particular panel, for example in list panels it may be
    used for editing values via command line with prefix `=`.

* `[ShiftEnter]`, `[CtrlA]`

    Opens a panel with provider properties of the current item. You can modify,
    add and delete some properties (depending on a provider). Example: key
    values of Registry provider.

* `[CtrlPgDn]`

    Opens a panel with the current object members. If a member type is
    *Property* you also can open its members by `[CtrlPgDn]` and so on. Use
    `[CtrlShiftM]` to switch member modes.

* `[CtrlG]`

    Apply command. Opens an input box and prompts to enter a command to be
    invoked for each object `$_` in selected in the panel.

* `[CtrlQ]`

    Quick view. Shows contents, properties and other information. Data may be
    not the same as information shown by `[F3]`.

* `[CtrlS]`

    Saves panel data. Implementation depends on a panel. E.g. a data panel
    commits changes to a database, an object panel exports objects to .clixml
    file, etc.

* `[AltF7]`

    Search. It is not really implemented.


*********************************************************************
## Object panel

[Contents]
[Power panel](#power-panel)

Table of any .NET or PowerShell objects, for example output of PowerShell
commands. You can send objects to the panel using `Out-FarPanel` cmdlet.

**Examples**

    ps: ps | Out-FarPanel

shows all processes in the Power panel with some properties in the description
column, then you can select one and view (`[F3]`) or quick view (`[CtrlQ]`) all its
properties - thus, it is similar to the *ProcList* plugin (see also *Panel-Process-.ps1*).

    ps: ps | sort WS | Out-FarPanel

the same but now objects (processes) are sorted by WS (working set); note that
you have to use unsorted Far mode to see the results sorted by you.

    ps: ps | sort WS | Out-FarPanel Name, @{Expression='WS'; Type='S'}

the same but with only two custom columns: Name and WS, where WS is mapped to
Far Size column.

You can collect objects in a panel, select, filter, sort them, view and edit
properties, sometimes delete and create properties. Then you can get them back
by `Get-FarItem` cmdlet (current, selected, all). Thus, object panel can be
used as intermediate visual storage for objects.

Note: you can collect together on the same panel objects of different types,
but in this case the engine may not know what columns should be shown, then
only names or string representation of objects is shown. This is not a bug.

---
**Special objects**

Object panels may recognize some object types and be able to perform some
operations by default. For example:

* Edit `[F4]`

    works for objects based on FileInfo (from `Get-*Item` cmdlets),
    `MatchInfo` (from `Select-String` cmdlet, found match is selected).

* Open `[Enter]`

    works for `GroupInfo` (from `Group-Object`), it opens yet another child
    object panel for the group, `[Esc]` returns you to the parent panel with
    groups.

Example commands to play with `[Enter]` on groups and `[F4]` on items:

    # Group files by extensions:
    ps: Get-ChildItem | Group-Object Extension | Out-FarPanel

    # Find string "throw" in files, group results by files:
    ps: Get-ChildItem | Select-String throw | Group-Object Path | Out-FarPanel

---
**Some data for experiments**

There are some data ready for tests and experiments. See the script
*Test-Zoo-.ps1* and comments. When you run this script you get 4 objects in a
panel: original .NET and PowerShell objects and their restored versions.

Tip: you can associate files *.clixml* with a command which imports objects
from to Object panel:

    ps: Import-Clixml (Get-FarPath) | Out-FarPanel #

or

    ps: Import-Panel-.ps1 (Get-FarPath) #

(`#` in the end tells not to add commands to history)

Note that only primitive data (basic value types and `byte[]`) keep their types on
importing from clixml (enough for many tasks). You can also export/import data
to/from .csv files, in this case most of imported data are strings.


*********************************************************************
## Item panel

[Contents]
[Power panel](#power-panel)

It shows PowerShell provider items in a specified provider path. Columns
(default or custom) show item properties. Some PowerShell providers:

- Registry (HKCU:, HKLM:)

    Access to the system registry and copy, move, delete, create and other
    operations on registry keys and values.

- Alias (Alias:)

    PowerShell aliases.

- Environment (Env:)

    Environment variables of the current process.

- FileSystem (C:, D:, ...)

    File system items, not a big deal in Far Manager, of course.

- Function (Function:)

    PowerShell functions in the current session.

- Variable (Variable:)

    PowerShell variables of the current session.

- Certificate (Cert:)

    Certificates for digital signatures.

- WSMan (WSMan:)

    WS-Management configuration information.

Other providers depend on imported PowerShell modules.

---
**Panel navigation and current PowerShell location**

If you change PowerShell location by a command, e.g. invoke commands like:

    ps: cd HKCU:\Software
    ps: Set-Location variable:\

then the panel contents is updated accordingly. And vice versa: if you navigate
in the panel to some location then PowerShell current location is changed, so
that in commands you may use current panel item names without full paths.

---
**How to open item or property panel at some location**

If you want to open a Power panel at the specified location from a script you
may use scripts *Go-To-.ps1* (not for *FileSystem*) and *Panel-Property-.ps1*
(for any provider). See comments and examples there.


*********************************************************************
## Folder tree

[Contents]
[Tree panel](#tree-panel)
[Power panel](#power-panel)

Provider folder tree panel is a tree panel where nodes represent provider
container items. It works for so called "navigation" providers. For example
standard PowerShell navigation providers are: *FileSystem*, *Registry*,
*Certificate*, and *WSMan*. Other providers depend on imported modules.

**Keys and actions**

* `[Enter]`

    As far this panel is for navigation providers, it is used mostly for
    navigation through an item tree. Note that quick search `[Alt+Letter]`
    works, too. When you reach an item you are looking for, press `[Enter]` to
    open an item panel for this location.

    For *FileSystem* provider `[Enter]` opens a standard Far file panel on the passive
    panel. This is done for convenience: standard Far file panel allows much more
    than its provider analogue. As a result the Folder tree panel is still opened
    and active, you can take a look at files on the passive panel and continue
    navigation in the tree.

    For all other providers `[Enter]` opens an item panel at the same active panel,
    as a child panel. When you exit it you return back to the folder tree.

* `[ShiftEnter]`, `[CtrlA]`

    Opens a panel with provider properties of the current item. Values are
    shown in the *Description* column. You can modify, add and delete some
    properties (depending on a provider). Example: key values of *Registry*
    provider.

    See [Tree panel](#tree-panel) for other tree panels keys used for navigation
    (expanding, collapsing nodes and etc.).


*********************************************************************
## Data panel

[Contents]
[Power panel](#power-panel)

**Warning: use this feature carefully, you can modify database data.**

Data panel shows database records selected by a SQL command and provides tools
used to modify and update data, insert and delete records.

**Keys and actions**

Data panel is built on the same engine as any other [Power panel](#power-panel),
so that you can find other keys not listed here that still work in Data panel.

* `[F7]`

    Inserts a new record into a table and opens a Member panel for editing data
    of the added record.

* `[F8]`

    Deletes selected records. If an error happens cursor is set to a record
    with an error.

* `[CtrlR]`

    Reads data from the database and fills the table. Note that not yet saved
    changes will be lost (usually not manual changes, e.g. changes done by
    scripts).

* `[CtrlS]`

    Commits all remaining changes to a database if they exist (for example if
    you have changed the table data by PowerShell commands or scripts). Table
    may have any number of modified, new and deleted records. `[CtrlS]` saves
    them all.

* `[PgDn]`, `[PgUp]`

    If their are pressed at the last or the first panel item respectively then
    they tell to show the next and previous page of records. Otherwise they
    work as usual. Use the `[F1]` menu in order to change paging settings.

* `[Enter]`, `[CtrlPgDn]`

    Opens a member panel for the current record. You may edit fields values.
    Use `[CtrlS]` to save your changes or `[Esc]` to return to the parent data
    panel (you will be prompted to save changes). On `[Enter]` some fields can
    open another (lookup) table so that values (or/and foreign keys) are taken
    from there (see *Test-Panel-DbNotes-.ps1*). If `[Enter]` is pressed in a
    lookup table panel it selects the value and closes the panel (`[CtrlPgDn]`
    still can be used to enter the record).

---
**Examples**

Almost complete set of data panel features is demonstrated by provided scripts,
at least it should be enough to learn how to create data panels.

**Utility scripts**

* *Panel-DbData-.ps1* creates a data panel by a single command with parameters
* *Panel-DbTable-.ps1* shows all connected tables and opens them in basic mode

**Demo scripts (see also About-Test.hlf)**

* *Test-Panel-DbCategories-.ps1* - simple data table with all operations
* *Test-Panel-DbNotes-.ps1* - complex data table with all operations and lookup field.
* *Test-Panel-DbText-.ps1* - read only table with data from two joined .CSV files (Jet 4.0)

---
**Notes**

Known issue: if you are about to delete or modify again just added and saved
record then at first you have to re-read the table data explicitly by `[CtrlR]`
(otherwise you can get concurrency error or record data can be incomplete and
etc.).

Once again: remember that a data panel is a kind of a [Power
panel](#power-panel), so that many keys and rules are the same.


*********************************************************************
## List panel

[Contents]
[Power panel](#power-panel)

List panel is used to view and modify properties of .NET objects, to view,
modify, add/remove dynamic properties, or to view all members including
methods. This panel consists of two columns: names and values/info.

An object being shown is exposed as `$Far.Panel.Host.Value`

**Keys and actions**

* `[Enter]`

    It is used to modify a property in the command line. If the command line is
    empty and the current property value can be represented as a single line of
    text, then `[Enter]` puts the value into the command line with a prefix
    `=`. If the command line is not empty and it starts with `=` then `[Enter]`
    treats the rest of the line as a new property value and, if it is possible,
    assigns it. See also `[F4]`, `[ShiftF8]`/`[ShiftDel]`, `[CtrlG]`.

* `[F3]`

    Opens a viewer to show property information, e.g. to find out whether a
    property is settable or not.

* `[F4]`

    Opens an editor to edit property value representable as multi-line text.
    You can open several editors. Properties are assigned on saving in editors.

* `[F8]`, `[Del]`

    Removes selected dynamic properties from the object or provider item. Note:
    this is not always allowed, it depends on objects, providers, and selected
    properties.

* `[ShiftF8]`, `[ShiftDel]`

    Sets null value to selected properties (kind of "deletes values"). Null
    values are shown as `<null>`.

* `[CtrlG]`

    Apply command. Opens an input box and prompts to enter a command to be
    invoked for the target object `$_` which members or properties are shown.
    This is used to invoke the target object methods and to assign results of
    expressions to properties.

* `[CtrlShiftM]`

    Switches panel modes: mode 1: properties and values (you can edit settable
    properties); mode 2: all public members and their information (read only
    but you can still use `[CtrlG]` to change the target object).

See [Power panel](#power-panel) for other keys.

---
**Notes**

It is not recommended to use member panels to view exposed internal objects
like `$Far`, `$Psf`, `$Far.Panel`, `$Far.Panel2`, and etc.


*********************************************************************
## Tree panel

[Contents]
[Power panel](#power-panel)

Tree panel is a kind of power panels dealing with PowerShell providers. It is
designed to display hierarchy of provider container items as a tree with
expandable nodes. Not all providers support container items, for example:
FileSystem and Registry do, Function and Variable do not.

**Keys and actions**

* `[Right]`

    Expands an item's children or, if it is already expanded or it is not
    expandable (Fill handler is null), moves the cursor to the next item. If an
    item was already expanded and then collapsed then its children are not
    refilled (for performance and keeping expanded children).

* `[AltRight]`

    Similar to `[Right]` but children items are refilled. Normally it is used
    to reflect external changes of source data.

* `[Left]`

    Collapses an item's children or moves the cursor to the parent item.

* `[AltLeft]`

    Similar to `[Left]` but child items are removed from the tree. It can be used
    to free not needed resources, or just to get refilled children when they
    are expanded next time.

* `[Alt+Letter]`

    Quick search. It should work fine, '+' and '-' in tree node names are
    ignored.

**View modes**

* `[Ctrl0]` - tree column and description status.
* `[Ctrl1]` - tree and description columns.


*********************************************************************
## Global objects

[Contents]

There are three main objects defined as global variables.

* `$Far`

    The instance of a class implementing `FarNet.IFar` interface. It provides
    access to all Far data and functionality via FarNet object model. See
    FarNet API documentation.

* `$Psf`

    The instance of `PowerShellFar.Actor` class exposing PowerShellFar features
    additional to FarNet features. See *FarNetAPI.chm* documentation. Note that
    PowerShellFar namespace provides public classes that can be created
    directly by `New-Object`.

* `$Host`

    The PowerShell host instance. It is not really useful in PowerShellFar. But
    you may check the property `$Host.Name`, it is "FarHost" in PowerShellFar.
    The script may choose how to work depending on a host.


*********************************************************************
## Profiles

[Contents]

PowerShellFar is configured via profiles with special names in the directory
*%FARPROFILE%\FarNet\PowerShellFar*. Each profile is invoked on the relevant
event, once per each session. Profiles are invoked in a session global scope.

Supported profiles:

- *Profile.ps1*
- *Profile-Editor.ps1*
- *Profile-Local.ps1*
- *Profile-Remote.ps1*

---
**Profile.ps1**

It is the main session profile invoked once on loading PowerShellFar.

IMPORTANT: It is invoked in the background for faster startup. This introduces
some limitations, actually easy to deal with:

* DO NOT add editor event handlers in the main profile, use the editor profile
  *Profile-Editor.ps1*.
* DO NOT call `$Far`. The profile is only for initialisation of the session,
  not for doing any work.
* Not terminating profile errors are not shown. A terminating error is shown in
  a GUI message box with a bare error message. Examine the variable `$Error` on
  profile troubleshooting after loading.

See also [Profile.ps1](#profileps1)

---
**Profile-Editor.ps1**

It is the editor profile invoked on the first use of editor. Normally it adds
editor event handlers to `$Far.AnyEditor`. The example profile is
[Profile-Editor.ps1](#profile-editorps1).

---
**Profile-Local.ps1 and Profile-Remote.ps1**

They are session profiles invoked on opening a local and remote interactive,
once per each new session. Note that the remote profile code is taken from the
local script but it is invoked in a remote workspace.

Not terminating profile errors are not shown. A terminating error is shown in a
standard message box with a bare error message. Examine the variable `$Error`
exactly *in these consoles* for full error information after opening.

*********************************************************************
## Settings

[Contents]

Settings are mostly user preferences and they are usually set in *Profile.ps1*.
The command to view or change settings temporarily is

    ps: Open-FarPanel $Psf.Settings

These settings are described in *FarNetAPI.chm*.

Not everything is configured via `$Psf.Settings`. There are other exposed
objects designed for configuration in the profile, e.g. `$Psf.Providers`.

See example [Profile.ps1](#profileps1).


*********************************************************************
## Commands output

[Contents]

**Console output**

Output of commands invoked from the command line with prefixes `ps:` is written
to the console, under panels if they are shown.

The output uses different colors depending on message types (errors, warnings,
etc.). Console output may be transcribed to a file, use `Start-Transcript` and
`Stop-Transcript` for starting and stopping and `Show-FarTranscript` for
viewing the output.

Note that console output may have unwanted screen effects on commands with UI
or may be difficult to see in the current context. Invoke such commands from
the command box or with `vps:` prefixes. On choosing a prefix for a command
with no output remember that errors also produce output to be shown.

Useful hotkeys in panels:

* `[CtrlUp]`, `[CtrlDown]` - change panels height
* `[CtrlAltShift]` - (press and hold) show the console
* `[CtrlO]`, `[CtrlF1]`, `[CtrlF2]` - hide and show panels

---
**Viewer output**

Output of commands invoked in the command box or Far Manager command line with
prefixes `vps:` is written to a temporary file and then shown in the internal
viewer. The viewer is modal in the modal context and modeless otherwise.

---
**Editor output**

Output of commands invoked in interactive is written to the current editor.
Note that local and remote consoles invoke commands asynchronously and output
does not block the UI. It is possible to switch to another window and return
later, the output produced in the background will be there.

---
**Discarded output**

Output of other code is discarded. For example, commands from the menu or event
handlers have no shown output. Failure messages are shown in error message
boxes. Non terminating errors and warnings are ignored.


*********************************************************************
## Background jobs

[Contents]

Background jobs are started from the command line or scripts by the cmdlet
`Start-FarJob`, see `Test-Job-.ps1` for examples.

**Rules**

Objects `$Far` and `$Psf` are not exposed for job code and they should not be
accessed in any other way because this is not thread safe. Accessing these
objects may lead to unpredictable results.

Background jobs should not rely on the process current directory: while they
are working it can be changed externally. A job should not change the current
directory, too. But PowerShell current location is totally under control of a
job, i.e. the command `Set-Location` is safe and this job location is not
visible or changed from outside.

Background jobs must not be interactive in any way, they should work absolutely
without a user until the end or failure. But you can perform interactive part
in the main thread (interactive data input or validation with error messages)
and then only, having all data ready, start the background part (example of
this approach is *Job-RemoveItem-.ps1* which is started in the main thread,
where it interacts with a user, then it starts a background job and exits).

In most cases it is fine to call external applications with or without
output, see [Console applications](#console-applications).

**Notes**

If you close Far and jobs still exist in the job list then for any job you are
prompted to abort it, wait for its exit, view its output or discard all jobs
and output. It is done with GUI message boxes and *Notepad.exe* because on
exiting Far UI is not available.

If it is not enough then there is another way which allows you to choose how to
proceed on jobs. The macro `[F10]` can get control of exit in panels: see
[Examples].


*********************************************************************
## Suffixes

[Contents]

Some scripts in Bench and Test folders ends with "-.ps1" and "..ps1". Why?

The suffixes designate that these scripts are not standard PowerShell scripts.

* Suffix "-.ps1"

    PowerShellFar scripts that should be invoked by FarHost. Normally they will
    fail if you invoke them by console host or by ISE host.

* Suffix "..ps1"

    PowerShellFar step unit scripts. Normally they should be invoked by
    PowerShellFar.Stepper for step sequence processing.

The suffixes are not mandatory for PowerShellFar scripts or step units, you may
use any names with or without suffixes. Suffixes are useful for distinguishing
between different script classes. Also, suffixes are effectively used for
assigning commands, see [File associations](:FileAssoc).

**Examples**

PowerShellFar scripts:

    mask     :  *-.ps1
    commands :
        ps: & (Get-FarPath) #

PowerShellFar steppers:

    mask     :  *..ps1
    commands :
        ps: Invoke-FarStepper -Path (Get-FarPath) #
        ps: Invoke-FarStepper -Path (Get-FarPath) -Ask #

Standard PowerShell scripts:

    mask     :  *.ps1|*[-.].ps1
    commands :
        PowerShell.exe -File "!\!.!"
        start PowerShell.exe -NoExit -NoLogo -File "!\!.!"

With the associations above when you press `[Enter]` (or another assigned key)
for scripts in the panel then standard scripts are invoked by PowerShell.exe,
PowerShellFar scripts are invoked by PowerShellFar and step units are invoked
by the stepper.


*********************************************************************
## Profile.ps1

[Contents]

The main session profile.

This is an example profile to start with, you may take what suits you there. It
should be copied to *%FARPROFILE%\FarNet\PowerShellFar*. Then you can change it
according to your preferences and activities, the script is only the example.

---
**Profile contents details**

    # Error action: 'Stop' is recommended to stop on errors immediately
    $ErrorActionPreference = 'Stop'

By default PowerShell does not stop on not terminating errors. Is this useful?
The answer depends on many factors including personal preferences. The author
recommends to use *Stop* for normal work, it is safer and more intuitive.

---

    # Aliases
    Set-Alias fff Find-FarFile -Description 'Finds the panel file'
    ...

The profile is a good place to define aliases. Some sample aliases are predefined.

---

    # The script invoked after interactive commands.
    $Psf.Settings.InteractiveEndOutputScript = 'Get-Date'

The script is invoked after each interactive command, its output is
converted to strings and written to the interactive editor.

---

    # Provider settings
    $Psf.Providers = ...

Define how provider data look in a panel; see API help for more details:
properties `Providers` (class `Actor`), `Columns` (class `ItemPanel`).

---

    # Preferences
    $Psf.Settings.PopupAutoSelect = $false
    ...

Here you can set some options of `$Psf.Settings`, mostly UI preferences.


*********************************************************************
## Profile-Editor.ps1

[Contents]

The editor profile.

The script is an example of the editor profile which is called once on the
first use of an editor. It should be in *%FARPROFILE%\FarNet\PowerShellFar*.

**Warning**

Read this topic carefully, view the script and then decide what you are going
to use from this script and what should be removed. If you are not absolutely
sure just don't use it at all. Besides, some operations can be invoked by
commands, menu, and macros.

The author uses this editor profile to set editor event handlers even in cases
when macros might be used more effectively. This is done deliberately in order
to be sure that handlers work fine, too. Other users may prefer to use macros.

- DO NOT use it as it is together with *HlfViewer*. Either disable the plugin
  or remove `[F1]` code from the script.

- DO NOT use it as it is with plugins processing mouse events in editors.
  Either disable the plugins or remove mouse code from the script.

**Events and actions**

This example editor profile covers the following events:

Keyboard events:

- `[F1]` - in a HLF file: save and show help for the current topic.

Mouse events:

- `LeftMove` - select to the moving location dynamically while moving.
- `RightClick` - shows a menu with some commands like Cut, Copy, and Paste.
- `Shift+LeftClick` - select from the last LeftClick position or from the cursor.

**See also**

The topic [Remove-EndSpace-.ps1](#remove-endspace-ps1) shows how to set different
event handlers for different file types.


*********************************************************************
## TabExpansion2.ps1

[Contents]

This is a replacement of the built-in PowerShell function.
PowerShell v3 `TabExpansion2` is replaced by *TabExpansion2.ps1*.
PowerShell v2 `TabExpansion` is replaced by *TabExpansion.ps1*.

The script has to be located in the PowerShellFar module home directory. It is
loaded on the first call. It replaces the built-in expansion function and adds
helpers.

In PowerShell v3 *TabExpansion2.ps1* reuses a lot of built-in completions and
supports extensions added by one or more `*ArgumentCompleters.ps1` profiles.

The script *Bench\ArgumentCompleters.ps1* is a sample profile. Use it as the
base for your own completers. See the script code and comments.

NOTE: *TabExpansion2.ps1* and *TabExpansion.ps1* can work in other PowerShell
hosts (console, ISE). All you need is to call the script once, normally in a
PowerShell profile.


*********************************************************************
## Remove-EndSpace-.ps1

[Contents]

(This is also an example of editor event handlers)

It removes white spaces from end of string property Text of any objects (note
that they can be even not related to Far). It can be used in Far editor:

    $Far.Editor.Lines | Remove-EndSpace- # all editor lines
    $Far.Editor.SelectedLines | Remove-EndSpace- # selected lines

A straightforward way of using this script is to use these commands from a menu
(PSF menu or Far user menu).

There is another way based on editor events and PowerShell handlers.

**Example task**

How to trim lines automatically on saving only .ps1 files (by the way, some end
spaces in PowerShell are syntax errors).

Put the following code into the editor profile:

    # add Opened handler for any file

    $Far.AnyEditor.add_Opened({ Handle-AnyEditorOpened })

    # this is a handler of Opened for any file:
    # if it is .ps1 file it adds Saving handler for it

    function Handle-AnyEditorOpened
    {
        if ([IO.Path]::GetExtension($this.FileName) -eq '.ps1') {
            $this.add_Saving({ Handle-EditorSaving })
        }
    }

    # This is a handler of Saving added to .ps1 files only in this example.
    # It calls Remove-EndSpace- to trim lines.

    function Handle-EditorSaving
    {
        $this.Lines | Remove-EndSpace-
    }

**Notes**

You can add more than one handler for any event.

PowerShell handlers do not use parameters, they use special variables:

- `$this` - sender of an event (editor in our example)
- `$_` - event argument (not used in our example)


*********************************************************************
## Job-RemoveItem-.ps1

[Contents]

Removing of large directories may be very time consuming. This script does the
job in the background, so that Far is not blocked during this time.

**Example**

Remove items selected in the active panel (e.g. you can use this from the Far
user menu, directly or by a macro):

    ps: Job-RemoveItem-.ps1 (Get-FarItem -Selected) #


*********************************************************************
## Search-Regex-.ps1

[Contents]

The script searches for a regex or a simple match in the specified source files
and sends found matches to a panel, so that you can open an editor just at
those lines with found text selected.

Search is performed in the background and results are dynamically sent to a
panel. You may work with found results immediately even with search in
progress.

If the parameter `Regex` is not defined you are prompted to enter it together
with other data.

**Input dialog controls**

- Pattern

    Specifies a regular expression pattern or a simple text to search for. See
    .NET documentation for regular expression details.

- Options

    Comma delimited regular expression and extra options.

    Standard .NET regular expression options: `None`, `IgnoreCase`,
    `Multiline`, `ExplicitCapture`, `Compiled`, `Singleline`,
    `IgnorePatternWhitespace`, `RightToLeft`, `ECMAScript`, `CultureInvariant`.

    Extra helper options: `SimpleMatch` tells that the pattern is a literal
    string, `WholeWord` tells to search for word bounds at the pattern start
    and end words.

- Input

    Any PowerShell command returning strings (file paths) or file system items
    (for example, but not only, from `Get-*Item` cmdlets). It is OK if some
    paths do not exist or some items are directories - such elements are
    ignored. Design and use commands carefully. If a command is not trivial try
    at first to compose it in the command line, test it, and only then use for
    a search.

- Groups

    Tells to panel found regex groups instead of full matches. It is ignored if
    "All text" is set.

- All text

    Tells to read and process a file as a single string, not as a line set. In
    this case options Multiline and Singleline can be used (they do not have
    much sense if "All text" is off). Results are processed in the same way but
    a found match is not selected in the editor, only cursor is set at the
    beginning of it.

- Background input

    By default an input command is invoked in the main runspace, it can use
    defined variables, commands, and etc. If a command actually does not need
    this and it is going to take long time itself, `dir C:\ -Recurse -Include
    *.txt`, then it is more effective to run it in the background by setting
    this flag.

---
**Result panel keys**

* `[Enter]` - open an editor at the selected match position in the text.
* `[Esc]` - close the panel (asking confirmation).
* `[F1]` - opens this help topic.

---
**Examples of input commands in a dialog**

Search in .ps1 files in the current directory:

    dir . -Include *.ps1

The same but with all sub-directories:

    dir . -Include *.ps1 -Recurse

The above command are fine for background input, they do not use anything from
the current PowerShell session. Commands below cannot be used for background
input. But they can create useful inputs using the API

Search in all panel items (any panel: file, temp, etc.)

    Get-FarPath -All

Search in selected panel items

    Get-FarPath -Selected

Search in files from the edit history

    $Far.History.Editor() | %{$_.Name}

NOTE: If the history contains not available network paths then the search may
take ages and sometimes it is difficult to stop. This command removes network
paths from the input:

    $Far.History.Editor() | %{if ($_.Name -notmatch '^\\\\') {$_.Name}}

---
**Command line mode**

The script is started with no dialog if the parameter `Regex` is defined. In
this case options are also defined in the command and input items are either
piped to the script or specified by the parameter `InputObject`. If it is a
script block then it is invoked in the background for getting input items.

Example:

    ls *.ps1 | Search-Regex-.ps1 TODO 'IgnoreCase, WholeWord'

Ditto but items are collected in the background:

    Search-Regex-.ps1 TODO 'IgnoreCase, WholeWord' {ls *.ps1}

---
**Notes**

The script demonstrates useful techniques of using background jobs for
processing and panels for displaying results and further operations.


*********************************************************************
## Other scripts

[Contents]

All other scripts are described just in place, see documentation comments there
or you can just invoke standard help commands, for example:

    man -det Panel-Data-.ps1
    help -full Panel-Property-.ps1
    ...


*********************************************************************
## Frequently asked questions

[Contents]

**Q: How to make PowerShell code invocation from the command line easier?**

A: There are several options

* Easy prefix macro

    A macro that expands the empty command line to 'ps: '.
    See *PowerShellFar.macro.lua*, `[Space]`.

* Easy invoke macro

    Type and run without prefix using a macro associated with the menu command
    "Invoke selected". See *PowerShellFar.macro.lua*, `[F5]`.

    A bonus: if a command fails then its text is still in the command line and
    the caret position is the same. This is useful on composing and correcting
    lengthy commands.


*********************************************************************
## Console applications

[Contents]

Since 2.2.10 output of invoked in PSF console applications is not written to
the console but merged with other commands output. Thus, in most cases it is
fine to call external applications with output from the command line, scripts,
interactive, and even background jobs.

Still, console applications with user interaction and applications operating on
console directly should not be used. Run them in Far or Cmd. This is especially
important for asynchronous interactive editors and background jobs.


*********************************************************************
## Command and macro examples

[Contents]

**Command examples**

Some examples are just demos but some of them may be practically useful as Far
user menu commands (do not forget to add space and # to the end if you do not
want a command to be added to the history). Examples with panels should be run
from panels.

---
Show three versions: Far, FarNet, PowerShellFar:

    ps: "Far $($Far.FarVersion)`nFarNet $($Far.FarNetVersion)`nPowerShellFar $($Host.Version)"

---
Do some math, keep results in variables, use them later

    ps: $x = [math]::sqrt([math]::pi)
    ps: $y = [math]::sqrt(3.14)
    ps: $x - $y

---
Add the current folder to the system path for this Far session (.NET example: static methods and properties)

    ps: [Environment]::SetEnvironmentVariable('PATH', $env:PATH + ';' + [Environment]::CurrentDirectory, 'Process')

---
Open selected files in editor at once

    ps: Get-FarPath -Selected | Start-FarEditor

---
View *.log* files one by one, don't add to history

    ps: Get-Item *.log | Start-FarViewer -Modal -DisableHistory

---
Find string "alias" in .ps1 files, show list of found lines and open editor at
the selected line (guess why these commands are the same):

    ps: Get-Item *.ps1 | Select-String alias | Out-FarList | Start-FarEditor
    ps: Get-Item *.ps1 | Select-String alias | Out-FarList | %{ Start-FarEditor $_.Path $_.LineNumber }

---
Show PSF settings in the member panel

    ps: Open-FarPanel $Psf.Settings

---
Show list of names and command lines of running processes and then show the selected process member panel (WMI)

    ps: Get-WmiObject Win32_Process | Out-FarList -Text { $_.Name + ' ' + $_.CommandLine } | Open-FarPanel

---
Show available scripts and their help synopsis in description column

    ps: Get-Command -Type ExternalScript | Get-Help | Out-FarPanel -Columns Name, Synopsis

---
**Macro examples**

PowerShell commands can be invoked from macros using the `Plugin.Call`
function. The first argument is the FarNet GUID, the second argument consists
of 0-2 colons, a command prefix, a colon, and a command. See the FarNet manual
for the roles of leading colons.

---
`[F10]` in the *Panels* area: safe exit without killing running PowerShellFar
background jobs. Standard PowerShell background jobs are not checked. If there
are jobs then the job menu is shown.

    if not Plugin.Call("10435532-9BB3-487B-A045-B0E6ECAAB6BC", ":ps: $Far.Quit()") then Keys("F10") end

---
This macro in the *Common* area calls the *Menu-Favorites-.ps1*. The leading
colon tells to call it as async job, e.g. to make macros working in the menu.

    Plugin.Call("10435532-9BB3-487B-A045-B0E6ECAAB6BC", ":ps: Menu-Favorites-.ps1")

---
This macro invokes the *Clear-Session.ps1* script. The macro uses the PSF
prefix `vps:` in order to show command output in the viewer:

    Plugin.Call("10435532-9BB3-487B-A045-B0E6ECAAB6BC", "vps: Clear-Session.ps1 -Verbose")

---
Scripts opened in the editor can be invoked in the current session by `[F5]`.
The key is hardcoded but yet another key still can be used for the same job
with this macro:

    Plugin.Call("10435532-9BB3-487B-A045-B0E6ECAAB6BC", "vps: $Psf.InvokeScriptFromEditor()")

*********************************************************************
