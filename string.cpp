#ifndef STRING_H
#define STRING_H

#include <iostream>
#include <cstring>
#include <algorithm>


class String {

private:
    char* content;
    size_t realsize;
    size_t size;

    void fix_size(size_t capacity) {
        if (realsize <= capacity || (capacity * 4) + 2 <= realsize){
            while (realsize <= capacity)  realsize *= 2;
            while ((capacity * 4) + 2 <= realsize) realsize /= 2;
            char* new_str = new char[realsize];
            std::fill(new_str,new_str + realsize,'0');
            size_t min = std::min(size,capacity);
            std::copy(content,content+min,new_str);
            content = new_str;
        }
        size = capacity;
        content[size] = '\0';
    }


    void change_str(const char* data,size_t start1,size_t start2,size_t newlen, size_t len) {
        fix_size(newlen);
        std::copy(data+start2,data+start2+len,content+start1);
    }


public:

    friend std::ostream& operator<<(std::ostream& out, const String& towrite);

    friend std::istream& operator>>(std::istream& in, String& toread);

    
    String(const char* data): String() {
        change_str(data,0,0,strlen(data),strlen(data));
    }

    String(char c) :String(){
        size = 1;
        realsize = 4;
        content = new char[realsize];
        content[0]=c;
        content[1]='\0';

    }


    String(){
        content = new char[1];
        content[0] = '\0';
        realsize = 1;
        size = 0;
    }


    String(size_t size, char c): String() {
        this->size = size;
        this->realsize = 4*size;
        content = new char[realsize];
        std::fill(content,content + size,c);
        content[size] = '\0';
    }

    String(const String& str): String(str.content) {}


    String& operator=(const String& str) {
        String copy(str);
        size = copy.size;
        realsize = copy.realsize;
        content = new char[realsize];
        std::copy(copy.content,copy.content + realsize ,content);
        return *this;
    }


    char& operator[](size_t i) {

        return content[i];

    }

    char operator[](size_t i) const {

        return content[i];

    }



    void push_back(char c) {
        fix_size(size + 1);
        content[size - 1] = c;

    }


    char& front() {

        return content[0];

    }

    void pop_back() {

        fix_size(size - 1);

    }

    char& back() {

        return content[size - 1];

    }

    char front() const {

        return content[0];

    }

    char back() const {

        return content[size - 1];

    }

    String& operator+=(const String& add) {

        change_str(add.content,size,0,size + add.size,add.size);
        return *this;

    }

    size_t length() const {

        return size;

    }


    String operator+(const String& end) const {
        String res = *this;
        res += end;
        return res;
    }

    bool operator==(const String& str) {

        return strcmp(content, str.content) == 0;

    }


    void clear() {
        content = new char[1];
        content[0] = '\0';
        realsize = 1;
        size = 0;
    }

    String substr(size_t beg, size_t cnt) const {
        if(beg + cnt < size){
        String res;
        res.change_str(content,0,beg,cnt,cnt);
        return res;
        }
        return "";
    }

    bool empty() const {

        return (size == 0) ? true: false;

    }

    size_t find(const String& str) const {
        if(size<str.size) return size;
        for (int i = 0; i <= static_cast<int>(size - str.size); ++i) {
            if (!strncmp(content + i, str.content, str.size)) return i;
        }

        return size;
    }

    size_t rfind(const String& str) const {
        if(size<str.size) return size;
        for (int i = static_cast<int>(size - str.size); i >= 0; --i) {
            if (!strncmp(content + i, str.content, str.size))  return i;
        }

        return size;
    }
    ~String() {

        delete[] content;

    }

};


std::istream& operator>>(std::istream& in, String& toread) {

    char symb;
    while ((symb = in.get()) != -1  && isspace(symb));
    if(symb == -1) return in ;
    toread.clear();
    toread.push_back(symb);
    while((symb = in.get()) != -1 && !isspace(symb)) toread.push_back(symb);
    return in;

}

std::ostream& operator<<(std::ostream& out, const String& towrite) {

    out << towrite.content;
    return out;
}

String operator+(char symb, const String& str) {

    return String(symb) + str;

}

#endif