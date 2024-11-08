import joblib
from collections import Counter, defaultdict
import math
import sys
import numpy as np
from typing import Dict, List, Set, Tuple, Literal

SimilarityMeasure = Literal['dot_product', 'cosine']

def compute_idf_for_tokens(tokens: Set[str], token_to_docs: Dict) -> Dict[str, float]:
    all_docs = set()
    for counter in token_to_docs.values():
        all_docs.update(counter.keys())
    N = len(all_docs)
    
    idf = {}
    for token in tokens:
        if token in token_to_docs:
            df = len(token_to_docs[token])
            idf[token] = np.log(N / df) if df > 0 else 0
    
    return idf

def create_token_to_index(tokens: Set[str]) -> Dict[str, int]:
    return {token: idx for idx, token in enumerate(sorted(tokens))}

def compute_tf_vectors(tokens: Set[str], token_to_docs: Dict, 
                      token_to_idx: Dict[str, int]) -> Tuple[np.ndarray, List[str]]:
    relevant_docs = set()
    for token in tokens:
        if token in token_to_docs:
            relevant_docs.update(token_to_docs[token].keys())
    
    doc_ids = sorted(relevant_docs)
    doc_to_idx = {doc: idx for idx, doc in enumerate(doc_ids)}

    tf_matrix = np.zeros((len(doc_ids), len(token_to_idx)))

    for token in tokens:
        if token in token_to_docs and token in token_to_idx:
            token_idx = token_to_idx[token]
            for doc, freq in token_to_docs[token].items():
                doc_idx = doc_to_idx[doc]
                tf_matrix[doc_idx, token_idx] = freq
    
    return tf_matrix, doc_ids

def compute_similarity_scores(query_vector: np.ndarray, 
                            doc_vectors: np.ndarray,
                            idf_vector: np.ndarray,
                            similarity: SimilarityMeasure = 'cosine') -> np.ndarray:
    query_tfidf = query_vector * idf_vector
    doc_tfidf = doc_vectors * idf_vector
    
    if similarity == 'cosine':

        query_norm = np.linalg.norm(query_tfidf)
        doc_norms = np.linalg.norm(doc_tfidf, axis=1)

        mask = (query_norm != 0) & (doc_norms != 0)
        scores = np.zeros(len(doc_tfidf))
        
        if mask.any():

            dot_products = np.dot(doc_tfidf, query_tfidf)
            scores[mask] = dot_products[mask] / (doc_norms[mask] * query_norm)
        
    else:  # dot_product
        scores = np.dot(doc_tfidf, query_tfidf)

    
    return scores

def search(query: str, token_to_docs: Dict, 
          similarity: SimilarityMeasure = 'cosine') -> List[Tuple[str, float]]:
    query_tokens = Counter(query.lower().split())
    unique_tokens = set(query_tokens.keys())

    token_to_idx = create_token_to_index(unique_tokens)

    idf_dict = compute_idf_for_tokens(unique_tokens, token_to_docs)

    idf_vector = np.array([idf_dict.get(token, 0) for token in sorted(unique_tokens)])

    query_vector = np.zeros(len(token_to_idx))
    for token, freq in query_tokens.items():
        if token in token_to_idx:
            query_vector[token_to_idx[token]] = freq

    doc_vectors, doc_ids = compute_tf_vectors(unique_tokens, token_to_docs, token_to_idx)

    if len(doc_ids) == 0:
        return []

    scores = compute_similarity_scores(query_vector, doc_vectors, idf_vector, similarity)

    results = [(doc_ids[i], score) for i, score in enumerate(scores) if score > 0]

    return sorted(results, key=lambda x: x[1], reverse=True)

def prompt(token_to_docs):
    query = input("Enter your search query (empty line to exit): ").strip()

    if len(query) < 1:
        return
    elif len(query.split()) > 1:
        print("\nResults using Cosine Similarity:")
        cosine_results = search(query, token_to_docs, similarity='cosine')[:10]
        for doc, score in cosine_results:
            print(f"{score:.4f}\t{doc}")
    else:
        print("\nResults using Dot Product:")
        dot_results = search(query, token_to_docs, similarity='dot_product')[:10]
        for doc, score in dot_results:
            print(f"{score:.4f}\t{doc}")

    print()
    return prompt(token_to_docs)    #tail recursion

def main():
    import sys
    if len(sys.argv) < 2:
        sys.exit("Missing the inverse index. Call the program like python search.py InverseIndex.joblib")
    token_to_docs = joblib.load(sys.argv[1])

    prompt(token_to_docs)

if __name__ == "__main__":
    main()