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

#include <windows.h>
#include <stdio.h>

#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/alloc.h>
#include <caml/custom.h>
#include <caml/fail.h>
#include <caml/threads.h>

#define NETWORK_CATEGORY ((WORD)0x00000001L)


CAMLprim value stub_log_something(value message) {
  CAMLparam1(message);

  HANDLE hEventLog = NULL;
  LPSTR pInsertStrings[2] = {NULL, NULL};
  int id = 1;
  pInsertStrings[0] = strdup(String_val(message));
  hEventLog = RegisterEventSource(NULL, "EventLogMessages.dll");
  if (!hEventLog) {
    fprintf(stderr, "Failed to RES with GLE=%ld\n", GetLastError());
    goto err;
  }
  if (!ReportEvent(hEventLog, EVENTLOG_ERROR_TYPE, NETWORK_CATEGORY, id, NULL, 1, 0, pInsertStrings, NULL)) {
    fprintf(stderr, "Failed with GLE=%ld\n", GetLastError());
  }
err:
  free(pInsertStrings[0]);
  CAMLreturn(0);
}
