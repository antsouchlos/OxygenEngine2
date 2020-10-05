
#ifndef NAMESPACES_H_INCLUDED
#define NAMESPACES_H_INCLUDED

#include <types/OE_Libs.h>

using namespace std;

namespace lexical_analyzer {
    string getIso(string &str, string subStr, unsigned int start_loc = 0) {
        string result = "";

        bool b = false;
        for (unsigned int i = start_loc; (i < str.size() - subStr.size()) && (str.size() > subStr.size()); i++) {

            string subStr2 = "";
            for (unsigned int subi = i; subi < i + subStr.size(); subi++) {
                subStr2 += str.at(subi);
            }

            if (subStr2 == subStr){
                if (b) {
                    b = false;
                    break;
                } else {
                    b = true;
                    i += subStr.size();
                }
            }

            if (b)
                result += str.at(i);
        }

        return result;
    }

    string getToStr(string &str, string subStr, unsigned int start_loc = 0) {
        string result = "";

        bool b = true;
        for (unsigned int i = start_loc; (i < str.size() - subStr.size()); i++) {

            string subStr2 = "";
            for (unsigned int subi = i; subi < i + subStr.size(); subi++)
                subStr2 += str.at(subi);

            if (subStr2 == subStr) {
                b = false;
                break;
            }

            result += str.at(i);
        }

        if (b)
            result = str;

        return result;
    }

    string getEnclosed(string &str, string sub1, string sub2) {
        string result = "";

        int openCount = -1;

        bool b = false;
        for (unsigned int i = 0; (i < str.size() - sub2.size()) && (str.size() > sub1.size()) && (str.size() > sub2.size()); i++) {
            string subStr = "";

            if (i < str.size() - sub1.size()) {
                for (unsigned int subi1 = i; subi1 < i + sub1.size(); subi1++)
                    subStr += str.at(subi1);
            }

            if (subStr == sub1) {
                if (openCount < 0) {
                    openCount = 1;
                    b = true;
                } else {
                    openCount++;
                }
            }

            subStr = "";
            for (unsigned int subi2 = i; subi2 < i + sub2.size(); subi2++)
                subStr += str.at(subi2);

            if (subStr == sub2)
                openCount--;

            if (openCount == 0)
                break;

            if (b) {
                result += str.at(i);
            }

        }
        if (result.size() > 0)
            result.replace(0, sub1.size(), "");

        if (sub1 == sub2)
            result = str;

        return result;
    }


    int getStrLocation(string &str, string subStr,unsigned int start_loc = 0) {
        int result = -1;
        // added start_loc
        for (unsigned int i = start_loc; i < str.size()-subStr.size() + 1; i++) {

            string compstr = "";
            for (unsigned int subi = i; subi < i + subStr.size(); subi++)
                compstr += str.at(subi);

            if (compstr == subStr) {
                result = i;
                break;
            }
        }

        return result;
    }

    int getStrLocationPlusSubstr(string &str, string subStr, unsigned int start_loc = 0) {
        int result = -1;
        // added start_loc
        for (unsigned int i = start_loc; i < str.size()-subStr.size() + 1; i++) {

            string compstr = "";
            for (unsigned int subi = i; subi < i + subStr.size(); subi++)
                compstr += str.at(subi);

            if (compstr == subStr) {
                result = i;
                break;
            }
        }

        return result + subStr.size();
    }

    string replaceFirstStr(string &str, string subStr, string repStr = "") {
        string result = str;

        int loc = getStrLocation(str, subStr);
        if (loc > -1)
            result.replace(loc, subStr.size(), repStr);

        return result;
    }

    int countStr(string &str, string subStr) {
        int result = 0;

        for (unsigned int i = 0; (i < str.size()-subStr.size()+1) && (str.size() > subStr.size()); i++) {

            string subStr2 = "";
            for (unsigned int subi = i; subi < i + subStr.size(); subi++) {
                subStr2 += str.at(subi);
            }
            if (subStr2 == subStr)
                result++;
        }

        return result;
    }

    int countStr(string &str, string subStr, size_t start_loc, size_t end_loc = 0) {
        int result = 0;
        size_t max_limit = str.size()-subStr.size()+1;
        if(end_loc != 0)
            max_limit = end_loc;
        cout << max_limit << endl;
        for (size_t i = start_loc; (i < max_limit && (str.size() > subStr.size())); i++) {

            string subStr2 = "";
            for (size_t subi = i; subi < i + subStr.size(); subi++) {
                subStr2 += str.at(subi);
            }
            if (subStr2 == subStr)
                result++;
        }

        return result;
    }
}

#endif // NAMESPACES_H_INCLUDED



