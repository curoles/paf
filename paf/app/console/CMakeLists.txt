# `juce_add_console_app` adds an executable target with the name passed as the first argument.
# This target is a normal CMake target, but has a lot of extra properties
# set up by default. This function accepts many optional arguments. Check the readme at
# `docs/CMake API.md` in the JUCE repo for the full list.

juce_add_console_app(paf
    PRODUCT_NAME "Play Audio File"  # The name of the final executable, which can differ from the target name
    COMPANY_NAME "Igor Lesik"
)

#juce_add_modules(
    #ALIAS_NAMESPACE xxx
    #${CMAKE_SOURCE_DIR}/3dparty_xxx/modules/xxx
    #)

# `juce_generate_juce_header` will create a JuceHeader.h for a given target, which will be generated
# into the build tree. This header should be included with `#include <JuceHeader.h>`. The include
# path for this header will be automatically added to the target. The main function of the
# JuceHeader is to include all the JUCE module headers for a particular target; if you're happy to
# include module headers directly, you probably don't need to call this.

juce_generate_juce_header(paf)

target_sources(paf
    PRIVATE
    PafMain.cpp PafApp.cpp
)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
target_compile_options(paf
    PRIVATE
        -Werror
)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
target_compile_options(paf
    PRIVATE
        /W1 #/Wall #/WX
    PUBLIC
        /ZI
)
target_link_options(paf
    PUBLIC
        /INCREMENTAL
)
endif()

# `target_compile_definitions` adds some preprocessor definitions to our target. In a Projucer
# project, these might be passed in the 'Preprocessor Definitions' field. JUCE modules also make use
# of compile definitions to switch certain features on/off, so if there's a particular feature you
# need that's not on by default, check the module header for the correct flag to set here. These
# definitions will be visible both to your code, and also the JUCE module code, so for new
# definitions, pick unique names that are unlikely to collide! This is a standard CMake command.

target_compile_definitions(paf
    PRIVATE
        # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
        JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_console_app` call
        JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_console_app` call
)

# If the target needs extra binary assets, they can be added here. The first argument is the name of
# a new static library target that will include all the binary resources. There is an optional
# `NAMESPACE` argument that can specify the namespace of the generated binary data class. Finally,
# the SOURCES argument should be followed by a list of source files that should be built into the
# static library. These source files can be of any kind (wav data, images, fonts, icons etc.).
# Conversion to binary-data will happen when the target is built.

# juce_add_binary_data(PafAppData SOURCES ...)

# `target_link_libraries` links libraries and JUCE modules to other libraries or executables. Here,
# we're linking our executable target to the `juce::juce_core` module. Inter-module dependencies are
# resolved automatically. If you'd generated a binary data target above, you would need to link to
# it here too.

target_link_libraries(paf
    PRIVATE
        # PafAppData            # If you'd created a binary data target, you'd link to it here
        juce::juce_core
        juce::juce_audio_basics
        juce::juce_audio_devices
        juce::juce_audio_formats
        juce::juce_audio_utils
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_warning_flags
)

