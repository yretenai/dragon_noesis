# fmt_dragon/src

The directory is set up in a specific way that doesn't really make sense.
Don't ask.

- The root folder (`/src`) is shared across all targets.
- Each target should have their own folder
    (i.e. dragon_lumberyard has `/src/lumberyard`)
- Each target should have a test tool in the test folder 
    (i.e. dragon_lumberyard has `/src/tool/lumberyard`)

## Debugging Noesis

Tell CMake to use uses and to install to the noesis folder
    (`-DCMAKE_INSTALL_PREFIX=C:\path\to\noesis -DUSE_NOESIS=1`)

Then when launching the plugin dll, make your IDE launch `noesis.exe`.
