(*
 * Copyright (c) 2016 Docker Inc
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 *)

(** Allow an application to log via the Windows event log

The Windows event log is similar to syslog on Unix systems.

Example:

{[
  (* TODO *)
]}

For context, please read the following documents:

{ol
{li
{{https://msdn.microsoft.com/en-us/library/aa363679(v=vs.85).aspx}
ReportEvent API documentation}
}
}
*)

type t
(** An event log handle, see [register] *)

val register: ?server:string -> string -> t
(** [register server source] registers the source named [source] with the
    event log on server [server]. If [server] is None then the local event
    log is used. *)

type ty = [
  | `Success
  | `Audit_failure
  | `Audit_success
  | `Error
  | `Information
  | `Warning
]
(** Type of event to be logged *)

val report: t -> ty -> int -> int -> string array -> unit
(** [report t ty category event strings] reports an event to the log [t]. The
    event has a global "type", as well as source-specific category and event ids.
    Each event takes an array of "insertion strings" -- the system log viewer
    will look up the category, event and user's language setting in the resource
    (.exe or .dll) associated with the event source in the registrty, and then
    "insert" the strings as parameters inside the template. *)

val log_something: string -> unit
