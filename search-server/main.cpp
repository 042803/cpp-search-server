#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

struct Document {
    int id;
    double relevance;
};

const int MAX_RESULT_DOCUMENT_COUNT = 5;

std::string ReadLine() {
    std::string s;
    std::getline(std::cin, s);
    return s;
}

int ReadLineWithNumber() {
    int result = 0;
    std::cin >> result;
    ReadLine();
    return result;
}

class SearchServer {
    
public:
    
    void SetStopWords(const std::string& text) {
        
        for (const std::string& word : SplitIntoWords(text)) {
            stop_words_.insert(word);
        }
    }

    void AddDocument(int document_id, const std::string& document) {
        
        const std::vector<std::string> words = SplitIntoWordsNoStop(document);
        int words_count = words.size();
        
        for (const std::string& word : words) {
            word_to_documents_[word][document_id] += 1.0 / words_count;
        }
        
        document_count_++;
    }

    std::vector<Document> FindTopDocuments(const std::string& raw_query) const {
        
        const std::set<std::string> query_words = ParseQuery(raw_query);
        auto matched_documents = FindAllDocuments(query_words);

        std::sort(matched_documents.begin(), matched_documents.end(),
             [](const auto& lhs, const auto& rhs) {
                 return lhs.second > rhs.second;
             });
        
        if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
            matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
        }

        std::vector<Document> results;
        
        for (const auto& [document_id, relevance] : matched_documents) {
            results.push_back({document_id, relevance});
        }

        return results;
    }

private:
    
    std::map<std::string, std::map<int, double>> word_to_documents_;
    std::set<std::string> stop_words_;
    int document_count_ = 0;

    
    std::vector<std::pair<int, double>> FindAllDocuments(const std::set<std::string>& query_words) const {
        std::map<int, double> document_to_relevance;

        for (const auto& word : query_words) {

            if (word_to_documents_.count(word) > 0) {
                const double idf = std::log(document_count_ / static_cast<double>(word_to_documents_.at(word).size()));

                for (const auto& [document_id, tf] : word_to_documents_.at(word)) {
                    document_to_relevance[document_id] += tf * idf;
                }
            }
        }

        for (const auto& word : stop_words_) {
            
            if (word_to_documents_.count(word) > 0) {
                
                for (const auto& [document_id, _] : word_to_documents_.at(word)) {
                    document_to_relevance.erase(document_id);
                }
            }
        }

        std::vector<std::pair<int, double>> result;

        for (const auto& [document_id, relevance] : document_to_relevance) {
            result.push_back({document_id, relevance});
        }

        return result;
    }

    std::set<std::string> ParseQuery(const std::string& text) const {
        std::set<std::string> query_words;

        for (const std::string& word : SplitIntoWordsNoStop(text)) {
            query_words.insert(word);
        }

        return query_words;
    }

    std::vector<std::string> SplitIntoWords(const std::string& text) const {
        std::vector<std::string> words;
        std::string word;

        for (const char c : text) {
            
            if (c == ' ') {
                
                if (!word.empty()) {
                    words.push_back(word);
                    word.clear();
                }

            } else {
                word += c;
            }
        }

        if (!word.empty()) {
            words.push_back(word);
        }

        return words;
    }

    std::vector<std::string> SplitIntoWordsNoStop(const std::string& text) const {
        std::vector<std::string> words;

        for (const std::string& word : SplitIntoWords(text)) {
            
            if (!IsStopWord(word)) {
                words.push_back(word);
            }
        }

        return words;
    }

    bool IsStopWord(const std::string& word) const {
        return stop_words_.count(word) > 0;
    }
};

SearchServer CreateSearchServer() {
    SearchServer search_server;
    search_server.SetStopWords(ReadLine());

    const int document_count = ReadLineWithNumber();
    
    for (int document_id = 0; document_id < document_count; ++document_id) {
        search_server.AddDocument(document_id, ReadLine());
    }

    return search_server;
}

int main() {
    const SearchServer search_server = CreateSearchServer();

    const std::string query = ReadLine();
    const std::vector<Document> results = search_server.FindTopDocuments(query);

    for (const auto& [document_id, relevance] : results) {
        std::cout << "{ document_id = " << document_id << ", "
             << "relevance = " << relevance << " }" << std::endl;
    }

    return 0;
}
   


