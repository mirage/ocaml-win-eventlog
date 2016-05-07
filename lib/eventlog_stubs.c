/*
 * Copyright (C) 2016 Docker Inc
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
 */
#include <stdio.h>
#include <string.h>

#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#else
/* For debugging on Unix */
#define HANDLE int*
#define BOOL int
#define TRUE 1
#define FALSE 0
#warning "This isn't a Windows platform: there is no real eventlog support here"
BOOL CloseHandle(HANDLE h){
  fprintf(stderr, "CloseHandle %d\n", (int)h);
  return TRUE;
}
#define LPCTSTR char*
#define DWORD int
DWORD GetLastError(){
  return 0;
}
HANDLE RegisterEventSource(LPCTSTR lpUNCServerName, LPCTSTR lpSourceName){
  fprintf(stderr, "RegisterEventSource(%s, %s)\n", lpUNCServerName, lpSourceName);
  return (HANDLE)1;
}
void win32_maperr(DWORD err){
  return;
}
#endif

#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/alloc.h>
#include <caml/custom.h>
#include <caml/fail.h>
#include <caml/threads.h>
#include <caml/unixsupport.h>

#define Eventlog_val(v) (*((HANDLE *) Data_custom_val(v)))

static void eventlog_finalize(value v) {
  CloseHandle(Eventlog_val(v));
}

static struct custom_operations eventlog_ops = {
  "djs55.eventlog",
  eventlog_finalize,
  custom_compare_default,
  custom_hash_default,
  custom_serialize_default,
  custom_deserialize_default
};

static value alloc_eventlog(HANDLE h) {
  value v = alloc_custom(&eventlog_ops, sizeof(HANDLE), 0, 1);
  Eventlog_val(v) = h;
  return v;
}

#define Val_None 0
#define Val_Some 1

CAMLprim value stub_register_event_source(value server_opt, value source) {
  CAMLparam2(server_opt, source);
  LPCTSTR lpUNCServerName = NULL;
  LPCTSTR lpSourceName = NULL;
  DWORD error = 0;
  HANDLE h = NULL;
  if (Field(server_opt, 0) == Val_Some) {
    lpUNCServerName = strdup(String_val(Field(server_opt, 1)));
  }
  lpSourceName = strdup(String_val(source));
  h = RegisterEventSource(lpUNCServerName, lpSourceName);
  if (h == NULL) {
    error = GetLastError();
  }
  free(lpUNCServerName);
  free(lpSourceName);
  if (h == NULL) {
    win32_maperr(error);
    uerror("RegisterEventSource", Nothing);
  }
  CAMLreturn(alloc_eventlog(h));
}

CAMLprim value stub_log_something(value message) {
  CAMLparam1(message);

#ifdef WIN32
  HANDLE hEventLog = NULL;
  LPSTR pInsertStrings[2] = {NULL, NULL};
  int id = 1;
  pInsertStrings[0] = strdup(String_val(message));
  hEventLog = RegisterEventSource(NULL, "OcamlEventLog.dll");
  if (!hEventLog) {
    DWORD err = GetLastError();
    win32_maperr(err);
    uerror("RegisterEventSource", Nothing);
    goto err;
  }
  WORD category = (WORD)0x00000001L;
  if (!ReportEvent(hEventLog, EVENTLOG_ERROR_TYPE, category, id, NULL, 1, 0, pInsertStrings, NULL)) {
    fprintf(stderr, "Failed with GLE=%ld\n", GetLastError());
  }
err:
  free(pInsertStrings[0]);
#else
  caml_failwith("Windows eventlog not available on this platform");
#endif
  CAMLreturn(0);
}
