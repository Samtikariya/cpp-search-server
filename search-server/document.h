﻿// в качестве заготовки кода используйте последнюю версию своей поисковой системы
#pragma once
#include <iostream>


enum class DocumentStatus {
	ACTUAL,
	IRRELEVANT,
	BANNED,
	REMOVED,
};

struct Document {
	int id = 0;
	double relevance = 0.0;
	int rating = 0;

	Document() = default;

	Document(int id, double relevance, int rating);
};

std::ostream& operator<<(std::ostream& out, const Document& document);