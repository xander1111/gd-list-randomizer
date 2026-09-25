# List Randomizer

<img src="logo.png" width="150" alt="the mod's logo" />

A Geometry Dash mod that adds a wheel that can be spun to select a random level from an ingame list.


## Features

- Adds a wheel that you can spin to randomly pick a level from a list
- Levels on a wheel can be assigned weights, affecting their probability of being chosen
- Levels can be disabled, removing them as an option on the wheel
- Levels can automatically be enabled and disabled according to filters
- Each wheel can be customized with different colors, as well as some additional wheel/theme options


### Planned Features

- Allow levels to be automatically assigned weight based on AREDL placement, GDDL ranking, or in-game difficulty rating


### Unplanned features

These are features I think would be cool to have, but that I'm not currently planning to implement for one reason or another.

- Customizable sounds
  - Shouldn't be that hard to implement, but finding sounds that work well for the wheel tick sound can be pretty
    difficult, and I don't know if that many people would want this in the first place.
- Allow customizing individual level colors
  - Shouldn't be that hard to implement, but I don't think I've ever seen someone use this feature on pickerwheel.com or
    wheelofnames.com over just using a theme so it doesn't really seem like there's a reason to add it.


## Build instructions

This mod is built with [Geode SDK](https://geode-sdk.org/install), which has installation instructions [here](https://docs.geode-sdk.org/).
If you have the Geode SDK installed, you can run this command to build:

```sh
geode build
```


## Acknowledgements

The audio files [selectLevel.ogg](resources/audio/selectLevel.ogg) and [tick.ogg](resources/audio/tick.ogg) are taken from Geometry Dash's own sound effect library, I did not create them nor do I own them.
I assume that their presence in the sound effect library means it's OK for me to use them in this way, but wanted to clarify anyway.
