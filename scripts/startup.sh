#!/bin/bash

# Export runtime Vulkan paths
export DYLD_LIBRARY_PATH='./lib'
export VK_ICD_FILENAMES='./lib/icd.d/MoltenVK_icd.json'

# Run the application
./app