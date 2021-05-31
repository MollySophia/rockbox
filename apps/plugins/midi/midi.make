#             __________               __   ___.
#   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
#   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
#   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
#   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
#                     \/            \/     \/    \/            \/
# $Id$
#

MIDISRCDIR := $(APPSDIR)/plugins/midi
MIDIBUILDDIR := $(BUILDDIR)/apps/plugins/midi

ROCKS += $(MIDIBUILDDIR)/midi.rock

MIDI_SRC := $(call preprocess, $(MIDISRCDIR)/SOURCES)
MIDI_OBJ := $(call c2obj, $(MIDI_SRC))

# add source files to OTHER_SRC to get automatic dependencies
OTHER_SRC += $(MIDI_SRC)

MIDICFLAGS = $(PLUGINFLAGS) -O2

$(MIDIBUILDDIR)/midi.rock: $(MIDI_OBJ)

# new rule needed to use extra compile flags
$(MIDIBUILDDIR)/%.o: $(MIDISRCDIR)/%.c
	$(SILENT)mkdir -p $(dir $@)
	$(call PRINTS,CC $(subst $(ROOTDIR)/,,$<))$(CC) $(MIDICFLAGS) -c $< -o $@

