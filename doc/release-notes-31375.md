New command line interface
--------------------------

A new `coinwow` command line tool has been added to make features more
discoverable and convenient to use. The `coinwow` tool just calls other
executables and does not implement any functionality on its own.  Specifically
`coinwow node` is a synonym for `coinwowd`, `coinwow gui` is a synonym for
`coinwow-qt`, and `coinwow rpc` is a synonym for `coinwow-cli -named`. Other
commands and options can be listed with `coinwow help`. The new tool does not
replace other tools, so all existing commands should continue working and there
are no plans to deprecate them.
