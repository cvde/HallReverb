# add binary assets
juce_add_binary_data(BinaryData
    SOURCES
        "VST_Compatible_Logo_Steinberg_with_TM_negative.svg"
)

# required for Linux
set_target_properties(BinaryData PROPERTIES POSITION_INDEPENDENT_CODE ON)
