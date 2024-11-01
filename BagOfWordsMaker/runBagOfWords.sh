#!/usr/bin/env bash
rm ./data/bagOfWords.txt

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
BATCH_SIZE=20
TEMP_FILES=()

process_folder() {
    local folder=$1
    local temp_output=$2
    files=()
    while IFS= read -r -d '' file; do
        files+=("$file")
        if [ ${#files[@]} -eq $BATCH_SIZE ]; then
            $SCRIPT_DIR/BagOfWordsMaker.exe "${files[@]}" >> "$temp_output"
            files=()  # Reset the array
        fi
    done < <(find "$folder" -type f \( -name "*.xml" -o -name "*.html" -o -name "*.htm" \) -print0)
    # Process any remaining files
    if [ ${#files[@]} -gt 0 ]; then
        $SCRIPT_DIR/BagOfWordsMaker.exe "${files[@]}" >> "$temp_output"
    fi
}

export -f process_folder
export TEMP_FILES

# Default to current directory if no arguments are provided
if [ $# -eq 0 ]; then
    set -- .
fi

#The input is runBagOfWords.sh [folder1, [folder2 ...]]
#The more folders you specify, the faster it can process them (because it does it in parallel)
#To do Texas State Law, you would do runBagOfWords.sh ./TexasStateLaw/*

# Run each folder in the background
for folder; do  #This loops over "$@"
    temp_output=$(mktemp)
    TEMP_FILES+=("$temp_output")
    process_folder "$folder" "$temp_output" &
done

# Wait for all background processes to finish
wait

files=()
for file in "${TEMP_FILES[@]}"; do
	files+=("$file")
	if [ ${#files[@]} -eq $BATCH_SIZE ]; then
		cat "${files[@]}" >> ./data/bagOfWords.txt
		files=()  # Reset the array
	fi
done
# Process any remaining files
if [ ${#files[@]} -gt 0 ]; then
	cat "${files[@]}" >> ./data/bagOfWords.txt
fi

# Combine all outputs into the final file
for temp_file in "${TEMP_FILES[@]}"; do
    rm "$temp_file"
done
