(lang dune 2.9)
(name win-eventlog)
(use_standard_c_and_cxx_flags true)
(generate_opam_files true)

(source (github mirage/ocaml-win-eventlog))
(license ISC)
(authors "David Scott" "Antonin Décimo")
(maintainers "dave@recoil.org")
(documentation "https://mirage.github.io/ocaml-win-eventlog/")
(package
 (name win-eventlog)
 (synopsis "Log via the Windows event log from OCaml programs")
 (description
"\| A low-level example:
"\|
"\| ```ocaml
"\| let log = Eventlog.register "Mirage.exe" in
"\| let category = 0 and event = 1 in
"\| Eventlog.report log `Success category event [|
"\|   "insertion string 1";
"\|   "insertion string 2";
"\| |]
"\| ```
 )
 (depends
  (ocaml (>= 4.08))
  result
  logs
  base-unix))
