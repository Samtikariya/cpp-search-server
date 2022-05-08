#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include "document.h"

void PrintDocument(const Document& document);

void PrintMatchDocumentResult(int document_id,
	const std::vector<std::string>& words,
	DocumentStatus status);

std::vector<std::string> SplitIntoWords(std::string& text);
std::vector<std::string_view> SplitIntoWords(std::string_view text);

template<typename StringContainer>
std::set<std::string, std::less<>> MakeUniqueNonEmptyStrings(const StringContainer& strings) {
	std::set<std::string, std::less<>> non_empty_strings;
	for (std::string_view str : strings) {
		if (!str.empty()) {
			non_empty_strings.emplace(str);
		}
	}
	return non_empty_strings;
}


