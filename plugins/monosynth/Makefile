#!/usr/bin/make -f
# Makefile for DISTRHO Plugins #
# ---------------------------- #
# Created by falkTX
#

# --------------------------------------------------------------
# Project name, used for binaries

NAME = monosynth

# --------------------------------------------------------------
# Files to build

OBJS_DSP = \
	ADSR.cpp.o \
	LFO.cpp.o \
	WaveTableOsc.cpp.o \
	WaveUtils.cpp.o \
	DistrhoPluginMonoSynth.cpp.o

# --------------------------------------------------------------
# Do some magic

include ../Makefile.mk

# --------------------------------------------------------------
# Enable all possible plugin types

ifeq ($(LINUX),true)
TARGETS += dssi
endif

TARGETS += lv2_dsp
TARGETS += vst

all: $(TARGETS)

# --------------------------------------------------------------
