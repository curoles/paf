# Introduction

***PAF*** is a collection of cross-platform audio tools and VST plugins.

- `paf` is a command-line soundfile player, recorder and signal generator;
  it is similar to [aplay and arecord](https://linux.die.net/man/1/aplay).
- `player` is GUI application to play audio files.
- `recorder` is GUI application to record sound.
- `generator` is GUI application to generate audio signals.

# `paf` Command-line Tool

`paf` command-line application is soundfile player, recorder and signal generator.

## Test Sound

Play a beep using default audio output device.

```shell
paf test
```

## List Audio Drivers and Devices


```shell
paf list
```

## Play Audio File

```shell
paf play <filename>
```

## Generate audio signal

```shell
paf generate
```

## Record sond

```shell
paf record
```



GIT submodules:

- `git submodule add --depth=1 https://github.com/juce-framework/JUCE.git`

Execute command `git submodule update --remote` to get latest version of the submodules.

You can read more about working with submodules [here](https://git-scm.com/book/en/v2/Git-Tools-Submodules).

Clone PAF repo:

```shell
git clone --recurse-submodules https://github.com/curoles/paf.git
```


Prerequisite Ubuntu:

```
sudo apt-get install libasound2-dev
sudo apt-get install libcurl4-openssl-dev
sudo apt-get install -y libwebkit2gtk-4.0-dev
```
