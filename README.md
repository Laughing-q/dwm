# Laughing-q's build of dwm
[DWM Wiki](https://wiki.archlinux.org/index.php?title=Dwm_(%E7%AE%80%E4%BD%93%E4%B8%AD%E6%96%87)) 

[DWM](https://dwm.suckless.org/) 

---
## FAQ

> What are the bindings?

This is suckless, mmmbud, the source code is the documentation! Check out [config.h](config.h).

Okay, okay, actually I keep a readme in `larbs.mom` for my whole system, including the binds here.
Press `super+F1` to view it in dwm (zathura is required for that binding).
I haven't kept `man dwm`/`dwm.1` updated though. PRs welcome on that, lol.

## Patches and features

- Clickable statusbar with my build of [dwmblocks](https://github.com/Laughing-q/dwmblocks).
- [dmenu](https://github.com/LukeSmithxyz/dmenu).
- Reads xresources colors/variables (i.e. works with `pywal`, etc.).
- scratchpad: Accessible with mod+shift+enter.
- New layouts: bstack, fibonacci, deck, centered master and more. All bound to keys `super+(shift+)t/y/u/h`.
- True fullscreen (`super+f`) and prevents focus shifting.
- Windows can be made sticky (`super+s`).
- stacker: Move windows up the stack manually (`super-I/K`).
- shiftview: Cycle through tags (`super+j/l`).
- vanitygaps: Gaps allowed across all layouts.
- swallow patch: if a program run from a terminal would make it inoperable, it temporarily takes its place to save space.

## Installation for newbs

```
git clone https://github.com/LukeSmithxyz/dwm
cd dwm
sudo make install
```

## Please install `libxft-bgra`!

This build of dwm does not block color emoji in the status/info bar, so you must install [libxft-bgra](https://aur.archlinux.org/packages/libxft-bgra/) from the AUR, which fixes a libxft color emoji rendering problem, otherwise dwm will crash upon trying to render one. Hopefully this fix will be in all libxft soon enough.

## Some other dependences(for ubuntu)
```shell
libx11-dev
libxinerama-dev
libxft-dev
libx11-xcb-dev
libxcb-res0-dev
```
Ubuntu install [libxft-bgra](https://github.com/uditkarode/libxft-bgra) 

