# Bindings to Windows Event Logging API

[![Build status](https://github.com/mirage/ocaml-win-eventlog/actions/workflows/main.yml/badge.svg)](https://github.com/mirage/ocaml-win-eventlog/actions)

This library allows you to log via the [Windows Event Logging][] API
from OCaml programs.

> Note
>
> The Event Logging API was designed for applications that run on the
> Windows Server 2003, Windows XP, or Windows 2000 operating
> system. In Windows Vista, the event logging infrastructure was
> redesigned. Applications that are designed to run on Windows Vista
> or later operating systems should use [Windows Event Log][] to log
> events.

A low-level example:

```ocaml
let log = Eventlog.register "Mirage.exe" in
let category = 0 and event = 1 in
Eventlog.report log `Success category event [|
  "insertion string 1";
  "insertion string 2";
|]
```

You may wish to use the [Logs][] reporter interface instead:

```ocaml
let log = Eventlog.register "Mirage.exe" in
Logs.set_reporter (Log_eventlog.reporter log ());

Log.err (fun f -> f "This is an error");
Log.info (fun f -> f "This is informational");
Log.debug (fun f -> f "This is lowly debugging data");
```

For more context, please read the [Reporting Events][] example.

Please note that this code will compile on non-Windows platforms, but
this is for debugging only.

[Windows Event Logging]: https://docs.microsoft.com/en-us/windows/win32/eventlog/event-logging
[Windows Event Log]: https://docs.microsoft.com/en-us/windows/win32/wes/windows-event-log
[Reporting Events]: https://docs.microsoft.com/en-us/windows/win32/eventlog/reporting-an-event
[Logs]: https://erratique.ch/software/logs
