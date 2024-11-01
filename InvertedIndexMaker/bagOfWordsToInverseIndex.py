from collections import Counter
import joblib

def parse_token_counts(filename):
    token_to_docs = {}
    doc_token_counts = {}
    current_title = None
    
    with open(filename, 'r') as f:
        lines = f.readlines()
        
    i = 0
    while i < len(lines):
        line = lines[i].strip()

        if not line:
            i += 1
            continue

        if (len(line.split()) > 4) or not any(char.isdigit() for char in line.split()[-1]):
            current_title = line

            i += 1
            things_on_that_line = lines[i].strip().split()
            if (len(things_on_that_line) != 2) or (not (things_on_that_line[0].isnumeric())) or (not (things_on_that_line[1].isnumeric())):
                import sys
                sys.exit(current_title + " is not a title? The next line isn't (numUniqueTokens, totalCount)")
            (num_unique_tokens, total_count) = things_on_that_line
            doc_token_counts[current_title] = (int(num_unique_tokens), int(total_count))

            i += 1
            continue

        try:
            token, count = line.split()
            count = int(count)

            if token not in token_to_docs:
                token_to_docs[token] = Counter()

            token_to_docs[token][current_title] = count
            
        except ValueError:
            print(f"Warning: Couldn't parse line: {line}")
            
        i += 1
    
    return {
        "InverseIndex": token_to_docs,
        "DocTokenCounts": doc_token_counts
    }

import sys

results = parse_token_counts(sys.argv[1])  # Replace with your filename

joblib.dump(results["InverseIndex"], '../data/InverseIndex.joblib')   #I'll capitalize it because it's an object.
joblib.dump(results["DocTokenCounts"], '../data/DocTokenCounts.joblib')   #I'll capitalize it because it's an object.