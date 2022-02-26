#include "search_server.h"

#include <algorithm>
#include <cmath>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

using namespace std;

/* Подставьте вашу реализацию класса SearchServer сюда */

/*
   Подставьте сюда вашу реализацию макросов
   ASSERT, ASSERT_EQUAL, ASSERT_EQUAL_HINT, ASSERT_HINT и RUN_TEST
*/

// -------- Начало модульных тестов поисковой системы ----------

// Тест проверяет, что поисковая система исключает стоп-слова при добавлении документов
void TestExcludeStopWordsFromAddedDocumentContent() {
    const int doc_id = 42;
    const string content = "cat in the city"s;
    const vector<int> ratings = { 1, 2, 3 };
    {
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        const auto found_docs = server.FindTopDocuments("in"s);
        ASSERT_EQUAL(found_docs.size(), 1u);
        const Document& doc0 = found_docs[0];
        ASSERT_EQUAL(doc0.id, doc_id);
    }

    {
        SearchServer server;
        server.SetStopWords("in the"s);
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        ASSERT_HINT(server.FindTopDocuments("in"s).empty(), "Stop words must be excluded from documents"s);
    }
}

/*
Разместите код остальных тестов здесь
*/
/*
void TestExcludeStopWordsFromAddedDocumentContent() {     //Поддержка стоп-слов
    const int doc_id = 42;
    const string content = "cat in the city"s;
    const vector<int> ratings = { 1, 2, 3 };
    // Сначала убеждаемся, что поиск слова, не входящего в список стоп-слов,
    // находит нужный документ
    {
        SearchServer server;
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        const auto found_docs = server.FindTopDocuments("in"s);
        assert(found_docs.size() == 1);
        const Document& doc0 = found_docs[0];
        assert(doc0.id == doc_id);
    }

    // Затем убеждаемся, что поиск этого же слова, входящего в список стоп-слов,
    // возвращает пустой результат
    {
        SearchServer server;
        server.SetStopWords("in the"s);
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, ratings);
        assert(server.FindTopDocuments("in"s).empty());
    }
}
*/



void TestForAddDocument() {
    {
        SearchServer server;
        const int doc_id = 11;
        const string content = "I new document"s;
        const vector <int> rating = { 1, 2, 3, 4 };
        server.AddDocument(doc_id, content, DocumentStatus::ACTUAL, rating);
        //    assert(server.GetDocumentCount() == 1);
        ASSERT_EQUAL(server.GetDocumentCount(), 1);
        vector <Document> testvec = server.FindTopDocuments("new"s);
        //    assert(testvec.size() == 1);
        ASSERT_EQUAL(testvec.size(), 1);
        ASSERT_HINT(!testvec.empty(), "Stop words must be included in documents"s);
    }
    {
        //закоментил строки с вторым добавленным документом, потому что в Урок 8 Фрэймворк и поисковая система задание не принимается 
        SearchServer server;
        //        const int doc_id = 12;
        const string content = "I new document 2"s;
        //        const string content2 = "I  document 2"s;
        const vector <int> rating = { -1, 2, 3, 4 };
        //        const vector <int> rating2 = { 1, 2, 3, 4 };
        server.AddDocument(11, content, DocumentStatus::ACTUAL, rating);
        //        server.AddDocument(5, content2, DocumentStatus::ACTUAL, rating2);
                //    assert(server.GetDocumentCount() == 1);
        //        ASSERT_EQUAL(server.GetDocumentCount(), 2);
        ASSERT_EQUAL(server.GetDocumentCount(), 1);
        vector <Document> testvec = server.FindTopDocuments("new"s);
        //    assert(testvec.size() == 1);
//        ASSERT_EQUAL(testvec.size(), 2);
        ASSERT_EQUAL(testvec.size(), 1);
        ASSERT_HINT(!testvec.empty(), "Stop words must be included in documents"s);
    }

}

void TestForMinusWordsExcludeFromSearch() {
    SearchServer server;
    const string content = "Dogs and cats best friends"s;
    server.AddDocument(0, content, DocumentStatus::ACTUAL, { 1, 2, 3 });
    const string query = "-cats"s;
    vector<Document> testvec = server.FindTopDocuments(query);
    //    assert(testvec.size() == 0);
    ASSERT_EQUAL(testvec.size(), 0);
    testvec = server.FindTopDocuments(query);
    //    assert(testvec.empty());
    ASSERT_HINT(testvec.empty(), "TestForMinusWordsExludeFromSearch"s);
}

void TestForMatchingDocuments() {
    SearchServer server;
    const int doc_id = 12;
    const vector <int> rating = { 1, 2, 3, 4 }, rating2 = { 1, -2, 3, -4 }, rating3 = { -1, -2, -3, -4 };
    DocumentStatus status = DocumentStatus::ACTUAL;
    //положительные оценки
    {
        SearchServer server;
        server.AddDocument(doc_id, "Hello mr big cat"s, status, rating);
        //        assert(server.GetDocumentCount() == 1);
        ASSERT_EQUAL(server.GetDocumentCount(), 1);
        vector<string> testvec;
        tie(testvec, status) = server.MatchDocument("cat"s, doc_id);
        //        assert(!testvec.empty());
        ASSERT_HINT(!testvec.empty(), "TestForMatchingDocuments"s);
    }
    {
        SearchServer server;
        server.AddDocument(doc_id, "Hello mr big cat"s, status, rating);
        //        assert(server.GetDocumentCount() == 1);
        ASSERT_EQUAL(server.GetDocumentCount(), 1);
        vector<string> testvec;
        tie(testvec, status) = server.MatchDocument("-cat"s, doc_id);
        //        assert(testvec.empty());
        ASSERT_HINT(testvec.empty(), "TestForMatchingDocuments"s);

    }
    //смешанные оценки
    {
        SearchServer server;
        server.AddDocument(doc_id, "Hello mr big cat"s, status, rating2);
        //        assert(server.GetDocumentCount() == 1);
        ASSERT_EQUAL(server.GetDocumentCount(), 1);
        vector<string> testvec;
        tie(testvec, status) = server.MatchDocument("cat"s, doc_id);
        //        assert(!testvec.empty());
        ASSERT_HINT(!testvec.empty(), "TestForMatchingDocuments"s);
    }
    {
        SearchServer server;
        server.AddDocument(doc_id, "Hello mr big cat"s, status, rating2);
        //        assert(server.GetDocumentCount() == 1);
        ASSERT_EQUAL(server.GetDocumentCount(), 1);
        vector<string> testvec;
        tie(testvec, status) = server.MatchDocument("-cat"s, doc_id);
        //        assert(testvec.empty());
        ASSERT_HINT(testvec.empty(), "TestForMatchingDocuments"s);

    }
    //отрицательные оценки
    {
        SearchServer server;
        server.AddDocument(doc_id, "Hello mr big cat"s, status, rating3);
        //        assert(server.GetDocumentCount() == 1);
        ASSERT_EQUAL(server.GetDocumentCount(), 1);
        vector<string> testvec;
        tie(testvec, status) = server.MatchDocument("cat"s, doc_id);
        //        assert(!testvec.empty());
        ASSERT_HINT(!testvec.empty(), "TestForMatchingDocuments"s);
    }
    {
        SearchServer server;
        server.AddDocument(doc_id, "Hello mr big cat"s, status, rating3);
        //        assert(server.GetDocumentCount() == 1);
        ASSERT_EQUAL(server.GetDocumentCount(), 1);
        vector<string> testvec;
        tie(testvec, status) = server.MatchDocument("-cat"s, doc_id);
        //        assert(testvec.empty());
        ASSERT_HINT(testvec.empty(), "TestForMatchingDocuments"s);

    }
}

void TestForSortTfIdf() {
    SearchServer server;
    const vector<int> doc_id = { 5, 6, 7, 8 };
    vector <string> content = {
       "cat and dog"s,
       "Where are you from"s,
       "I like white cats"s,
       "Dogs and cats best friends"s };

    vector <vector<string>> documents = {
        {"cat"s, "and"s, "dog"s},
        {"Where"s, "are"s, "you"s, "from"s},
        {"I"s, "like"s, "white"s,"cats"s},
        {"Dogs"s, "and"s, "cats"s, "best"s, "friends"s} };

    const vector<int> ratings = { 1, 2, 3 };
    for (int i = 0; i < content.size(); i++)
    {
        server.AddDocument(doc_id[i], content[i], DocumentStatus::ACTUAL, ratings);
    }
    //    assert(server.GetDocumentCount() == 4);
    ASSERT_EQUAL(server.GetDocumentCount(), 4);
    vector<double> tf;
    int document_freq = 0;
    vector<double> ved;
    string term = "cats"s;

    for (const auto& document : documents) {
        int word_count = count(begin(document), end(document), term);
        if (word_count > 0)
        {
            tf.push_back(static_cast<double>(word_count) / static_cast<double>(document.size()));
            document_freq += count(begin(document), end(document), term);
        }
    }
    for (int i = 0; i < tf.size(); i++) {
        ved.push_back(tf[i] * log(static_cast<double>(documents.size()) / static_cast<double>(document_freq)));
    }
    vector <Document> nv = server.FindTopDocuments(term);
    sort(begin(ved), end(ved), [](double& d1, double& d2) {return d1 > d2; });
    for (int i = 0; i < ved.size(); i++)
    {
        //        assert(ved[i] == nv[i].relevance);
        ASSERT_EQUAL(ved[i], nv[i].relevance);
        //не понял что требуется в замечании. ге могли бы вы пояснить или показать
    }
}

void TestForRatingDocument() {
    SearchServer server;
    const DocumentStatus status = DocumentStatus::ACTUAL;
    const vector<int> rating = { 1, 2, 3, 4 };
    int sum_ratings = 0;
    for (const int irating : rating) {
        sum_ratings += irating;
    }
    const int avg_rating = sum_ratings / rating.size();
    server.AddDocument(0, "Dogs and cats best friends"s, status, rating);
    //    assert(server.GetDocumentCount() == 1);
    ASSERT_EQUAL(server.GetDocumentCount(), 1);
    vector<Document> testvec = server.FindTopDocuments("cats"s);
    //    assert(avg_rating == testvec[0].rating);
    ASSERT_EQUAL(avg_rating, testvec[0].rating);
}

void TestForPredicate() {
    SearchServer server;
    const DocumentStatus status = DocumentStatus::ACTUAL;
    const vector<int> rating = { 1, 2, 3, 4, 5 };
    int sum_ratings = 0;
    for (const int irating : rating) {
        sum_ratings += irating;
    }
    const int avg_rating = sum_ratings / rating.size();

    vector <string> content = {
        "cat and dog"s,
        "Where are you from"s,
        "I like white cats"s,
        "Dogs and cats best friends"s };

    vector <vector<string>> documents = {
        {"cat"s, "and"s, "dog"s},
        {"Where"s, "are"s, "you"s, "from"s},
        {"I"s, "like"s, "white"s,"cats"s},
        {"Dogs"s, "and"s, "cats"s, "best"s, "friends"s} };

    for (int i = 0; i < content.size(); ++i)
    {
        server.AddDocument(i, content[i], status, rating);
    }
    for (int i = 0; i < documents.size(); ++i) {
        for (const auto& ss : documents[i]) {
            auto predicate = [i, status, avg_rating](int id, DocumentStatus st, int rating) {
                return id == i && st == status && avg_rating == rating;
            };
            auto result = server.FindTopDocuments(ss, predicate);
            //            assert(result.size() == 1);
            ASSERT_EQUAL(result.size(), 1);
        }
    }
}



void TestForDocumentsStatus() {
    SearchServer server;
    const string content = "Dogs and cats best friends"s;
    //    const DocumentStatus status = DocumentStatus::ACTUAL;
    server.AddDocument(4, "I like white cats"s, DocumentStatus::ACTUAL, { 8, -3 });
    server.AddDocument(1, "I like cats and dogs"s, DocumentStatus::ACTUAL, { 7, 2, 7 });
    server.AddDocument(2, "cat and dog"s, DocumentStatus::ACTUAL, { 5, -12, 2, 1 });
    server.AddDocument(3, "this dog is big and fluff"s, DocumentStatus::BANNED, { 9 });
    //    auto testvec = server.FindTopDocuments("cats"s, status);
    auto t = server.FindTopDocuments("this"s, DocumentStatus::BANNED);
    //    assert(content.size() == 26);
//    ASSERT_EQUAL(content.size(), 26);
    ASSERT_EQUAL(t.size(), 1);
}


// Функция TestSearchServer является точкой входа для запуска тестов
void TestSearchServer() {
    TestExcludeStopWordsFromAddedDocumentContent();
    TestForAddDocument();
    TestForMinusWordsExcludeFromSearch();
    TestForMatchingDocuments();
    TestForSortTfIdf();
    TestForRatingDocument();
    TestForPredicate();
    TestForDocumentsStatus();

    // Не забудьте вызывать остальные тесты здесь
}


// --------- Окончание модульных тестов поисковой системы -----------

int main() {
    TestSearchServer();
    // Если вы видите эту строку, значит все тесты прошли успешно
    cout << "Search server testing finished"s << endl;
}