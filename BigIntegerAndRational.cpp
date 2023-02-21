#ifndef BIGINTEGER_H_
#define BIGINTEGER_H_

#include<bits/stdc++.h>

using namespace std;

class BigInteger {
    private:
        string digits;
    bool sign;
    public:

        BigInteger();
    BigInteger(string);
    BigInteger(string, bool);
    BigInteger(int);
    // ~BigInteger();

    BigInteger operator = (const BigInteger & );
    bool operator == (const BigInteger & ) const;
    bool operator != (const BigInteger & ) const;
    bool operator > (const BigInteger & ) const;
    bool operator < (const BigInteger & ) const;
    bool operator >= (const BigInteger & ) const;
    bool operator <= (const BigInteger & ) const;

    BigInteger & operator += (const BigInteger & );
    BigInteger & operator -= (const BigInteger & );
    BigInteger & operator *= (const BigInteger & );
    BigInteger & operator /= (const BigInteger & );
    BigInteger & operator %= (const BigInteger & );

    explicit operator bool() const;

    BigInteger & operator++();
    BigInteger operator++(int);
    BigInteger & operator--();
    BigInteger operator--(int);

    void trim();
    BigInteger operator - () const;

    std::ostream & operator << (const BigInteger & );
    std::istream & operator >> (BigInteger & );

    BigInteger abs() const;
    BigInteger & operator[](int);

    string toString() const;

    private:
        string addition(string, string) const;
    string subtraction(string, string) const;
    string multiplication(string, string) const;
    pair < string, string > divmod(const BigInteger & ,
        const BigInteger & ) const;

};

BigInteger::BigInteger() {
    digits = "0";
    sign = 0;
}

BigInteger::BigInteger(string s) {
    sign = s[0] == '-';
    if (sign) digits = s.substr(1);
    else digits = s;
}

BigInteger::BigInteger(string s, bool sign) {
    digits = s;
    this -> sign = sign;
}

BigInteger::BigInteger(int n): BigInteger(to_string(n)) {}

void BigInteger::trim() {
    while (!digits.empty() && digits.front() == 48)
        digits.erase(0, 1);
    if (digits.empty()) {
        sign = 0;
        digits.push_back(48);
    }
}

BigInteger BigInteger::abs() const {
    return BigInteger(digits);
}

BigInteger BigInteger::operator = (const BigInteger & b) {
    digits = b.digits;
    this -> sign = b.sign;
    return *this;
}

bool BigInteger::operator == (const BigInteger & b) const {
    return ( * this).digits == b.digits &&
        ( * this).sign == b.sign;
}

bool BigInteger::operator != (const BigInteger & b) const {
    return !(( * this) == b);
}

bool BigInteger::operator < (const BigInteger & b) const {
    bool sign1 = this -> sign;
    bool sign2 = b.sign;
    bool f;

    if (sign1 != sign2) {
        return (sign1 && !sign2) ? true : false;
    } else {
        if (digits.size() < b.digits.size()) f = 1;
        else if (digits.size() > b.digits.size()) f = 0;
        else f = ( * this).digits < b.digits;
        if (sign) {
            return !f;
        }
        return f;
    }
}

bool BigInteger::operator <= (const BigInteger & b) const {
    return !(( * this) > b);
}

bool BigInteger::operator > (const BigInteger & b) const {
    return b < ( * this);
}

bool BigInteger::operator >= (const BigInteger & b) const {
    return !(( * this) < b);
}
BigInteger & BigInteger::operator++() {
    ( * this) += 1;
    return ( * this);
}

BigInteger BigInteger::operator++(int) {
    BigInteger old = ( * this);

    ++( * this);

    return old;
}

BigInteger & BigInteger::operator--() {
    ( * this) -= 1;
    return ( * this);

}

BigInteger BigInteger::operator--(int) {
    BigInteger old = ( * this);

    --( * this);

    return old;
}

BigInteger operator + (const BigInteger & a,
    const BigInteger & b) {
    BigInteger temp = a;
    temp += b;
    return temp;
}

BigInteger operator - (const BigInteger & a,
    const BigInteger & b) {
    BigInteger temp = a;
    temp -= b;
    return temp;

}

BigInteger::operator bool() const {
    return *this != 0;
}

BigInteger operator * (const BigInteger & a,
    const BigInteger & b) {
    BigInteger res = a;
    res *= b;
    return res;
}

BigInteger operator / (const BigInteger & a,
    const BigInteger & b) {

    BigInteger res = a;
    res /= b;
    return res;
}

BigInteger operator % (const BigInteger & a,
    const BigInteger & b) {

    BigInteger temp = a;
    temp %= b;
    return temp;

}

BigInteger & BigInteger::operator += (const BigInteger & b) {

    if (sign == b.sign) {
        this -> digits = addition(digits, b.digits);
        this -> sign = sign;
    } else {
        if (abs() < b.abs()) {
            this -> digits = subtraction(b.digits, digits);
            this -> sign = b.sign;
        } else {
            this -> digits = subtraction(digits, b.digits);
            this -> sign = sign;
        }
    }
    if (this -> digits == "0")
        this -> sign = false;

    return *this;

}

BigInteger & BigInteger::operator -= (const BigInteger & b) {

    return *this += (-b);
}

BigInteger & BigInteger::operator *= (const BigInteger & b) {

    this -> digits = multiplication(digits, b.digits);
    this -> sign = (sign ^ b.sign);

    if (this -> digits == "0")
        this -> sign = false;

    return *this;

}

BigInteger & BigInteger::operator /= (const BigInteger & b) {

    this -> digits = divmod( * this, b).first;
    this -> sign = sign ^ b.sign;

    if (this -> digits == "0")
        this -> sign = false;

    return *this;

}

BigInteger & BigInteger::operator %= (const BigInteger & b) {
    this -> digits = divmod( * this, b).second;
    this -> sign = sign ^ b.sign;

    if (this -> digits == "0")
        this -> sign = false;

    return *this;
}

BigInteger & BigInteger::operator[](int n) {
    return *(this + n);
}

BigInteger BigInteger::operator - () const {
    return ( * this == 0) ? * this : ( * this) * -1;
}

std::string BigInteger::toString() const {
    string nstr = (sign) ? "-" + digits : digits;
    return nstr;
}

string BigInteger::subtraction(string num1, string num2) const {
    string suprem = (num1.length() > num2.length()) ? num1 : num2;
    int size_diff = std::abs(static_cast < int > (num1.size() - num2.size()));

    num1.size() > num2.size() ? num2.insert(0, size_diff, 48) : num1.insert(0, size_diff, 48);
    int i = static_cast < int > (num1.length()) - 1;
    while (i >= 0) {
        if (num1[i] - num2[i] < 0) {
            num1[i] += 10;
            num1[i - 1]--;
        }
        suprem[i] = (num1[i] - num2[i]) + 48;
        --i;
    }

    while (suprem.front() == 48)
        suprem.erase(0, 1);
    if (suprem.empty()) suprem = "0";

    return suprem;
}

std::istream & operator >> (std::istream & in , BigInteger & bigInt) {
    std::string input; in >> input;
    bigInt = BigInteger(input);
    return in;
}

string BigInteger::addition(string num1, string num2) const {
    string suprem = (num1.length() > num2.length()) ? num1 : num2;
    char carry = 48;
    int size_diff = std::abs(static_cast < int > (num1.size() - num2.size()));

    num1.size() > num2.size() ? num2.insert(0, size_diff, 48) : num1.insert(0, size_diff, 48);

    int i = static_cast < int > (num1.size()) - 1;
    while (i >= 0) {
        suprem[i] = (carry - 48) + (num1[i] - 48) + num2[i];

        if (i != 0) {
            carry = suprem[i] > '9' ? '1' : 48;
            if (suprem[i] > '9') suprem[i] -= 10;
        }
        --i;
    }

    if (suprem[0] > '9') {
        suprem[0] -= 10;
        suprem.insert(0, 1, '1');
    }

    return suprem;
}

std::ostream & operator << (std::ostream & out,
    const BigInteger & bigInt) {
    out << bigInt.toString();
    return out;
}

string BigInteger::multiplication(string n1, string n2) const {
    if (n1.length() > n2.length()) n1.swap(n2);

    string res = "0";
    string temp;
    int current, carry;
    for (int i = static_cast < int > (n1.length()) - 1; i >= 0; --i) {
        temp = n2;
        current = n1[i] - 48;
        carry = 0;

        for (int j = static_cast < int > (temp.length()) - 1; j >= 0; --j) {

            temp[j] = ((temp[j] - 48) * current) + carry;
            carry = (temp[j] > 9) ? (temp[j] / 10) : 0;
            if (temp[j] > 9) temp[j] -= (carry * 10);
            temp[j] += 48;
        }

        if (carry > 0) temp.insert(0, 1, (carry + 48));
        temp.append((n1.length() - i - 1), 48);
        res = addition(res, temp);
    }

    while (res.front() == 48)
        res.erase(0, 1);
    if (res.empty()) res = "0";

    return res;
}

pair < string, string > BigInteger::divmod(const BigInteger & a,
    const BigInteger & b) const {
    BigInteger res;
    res.digits.resize(a.digits.size());
    BigInteger rem;

    for (size_t i = 0; i < a.digits.size(); ++i) {
        int val = 0, left = 0, right = 9;
        rem.digits.push_back(48);
        rem.trim();
        rem.digits.back() = a.digits[i];
        while (left <= right) {
            int mid = (left + right) / 2;
            BigInteger cur(0);
            cur = b.abs() * mid;
            if (cur.abs() <= rem.abs()) {
                val = mid;
                left = mid + 1;
            } else
                right = mid - 1;
        }

        res.digits[i] = val + 48;
        rem -= (b.abs() * val);
    }
    rem.trim();
    res.trim();

    return make_pair(res.digits, rem.digits);
}

class Rational {
    private:
        BigInteger num;
    BigInteger denom;

    void reduce();

    public:
        Rational();
    Rational(const BigInteger & );
    Rational(int);
    Rational(const Rational & m);
    Rational & operator = (const Rational & );
    Rational(const BigInteger & ,
        const BigInteger & );
    bool operator == (const Rational & ) const;
    bool operator != (const Rational & ) const;
    bool operator > (const Rational & ) const;
    bool operator < (const Rational & ) const;
    bool operator >= (const Rational & ) const;
    bool operator <= (const Rational & ) const;
    const Rational operator - () const;
    std::string asDecimal(size_t) const;
    explicit operator double() const;
    const std::string toString() const;
    Rational & operator *= (const Rational & );
    Rational & operator += (const Rational & );
    Rational & operator -= (const Rational & );
    Rational & operator /= (const Rational & );
};

Rational::Rational() {
    num = BigInteger(1);
    denom = BigInteger(1);

}
Rational::Rational(const BigInteger & n) {
    num = n;
    denom = BigInteger(1);
}

Rational::Rational(int n) {
    num = BigInteger(n);
    denom = BigInteger(1);
}

Rational::Rational(const Rational & m) {

    num = m.num;
    denom = m.denom;
}

Rational & Rational::operator = (const Rational & n) {
    this -> num = n.num;
    this -> denom = n.denom;
    return *this;
}

Rational::Rational(const BigInteger & a,
    const BigInteger & b) {
    if (b < 0) {
        num = BigInteger(-a);
        denom = BigInteger(-b);
    } else {
        num = BigInteger(a);
        denom = BigInteger(b);
    }
    reduce();
}

BigInteger GCD(BigInteger a, BigInteger b) {

    if (b > a)
        std::swap(a, b);

    while (b) {
        BigInteger copy = b;
        b = a % b;
        a = copy;
    }

    return a;
}

BigInteger LCD(BigInteger a, BigInteger b) {
    return (a.abs() * b.abs()) / GCD(a.abs(), b.abs());
}

void Rational::reduce() {

    BigInteger gcd = GCD(num.abs(), denom.abs());

    num /= gcd;
    denom /= gcd;

}
const std::string Rational::toString() const {
    string str;
    str += num.toString();
    if (denom != 1)
        str += '/' + denom.toString();
    return str;
}

Rational::operator double() const {
    return std::stod(asDecimal(10));
}

bool Rational::operator == (const Rational & number) const {
    if (num != number.num || denom != number.denom)
        return 0;
    return 1;
}
bool Rational::operator != (const Rational & number) const {
    return !( * this == number);
}
bool Rational::operator < (const Rational & number) const {

    return num * number.denom < denom * number.num;
}
bool Rational::operator <= (const Rational & number) const {
    return !( * this > number);
}

bool Rational::operator > (const Rational & number) const {
    return number < * this;
}

bool Rational::operator >= (const Rational & number) const {
    return !( * this < number);
}
const Rational Rational::operator - () const {
    Rational b = * this;
    if (b != 0) {
        b.num = -b.num;
    }
    return b;
}
std::string Rational::asDecimal(size_t precision) const {

    std::string res = "";
    if (num < 0) res += "-";
    res += (num.abs() / denom).toString();
    if (precision) {
        res += '.';
        BigInteger a = (num.abs() % denom);
        for (size_t i = 0; i < precision; ++i) {
            a *= 10;
        }
        a /= denom;
        std::string f = a.toString();
        while (f.back() == 48)
            f.pop_back();
        size_t m = f.size();
        for (size_t i = 0; i < precision - m; ++i)
            f = "0" + f;
        res += f;

    }

    return res;
}

const Rational operator * (const Rational & r1,
    const Rational & r2) {

    Rational res = r1;
    res *= r2;
    return res;
}

const Rational operator + (const Rational & a,
    const Rational & b) {
    Rational res = a;
    res += b;
    return res;
}

const Rational operator - (const Rational & a,
    const Rational & b) {
    Rational temp = a;
    temp -= b;
    return temp;
}
const Rational operator / (const Rational & a,
    const Rational & b) {
    Rational temp = a;
    temp /= b;
    return temp;

}

Rational & Rational::operator *= (const Rational & a) {

    ( * this).num *= a.num;
    ( * this).denom *= a.denom;
    reduce();
    return *this;

}
Rational & Rational::operator += (const Rational & a) {
    BigInteger common = LCD(denom.abs(), a.denom.abs());
    this -> num = num * (common / denom) + (a.num) * (common / a.denom);
    this -> denom = common;
    reduce();
    return *this;
}
Rational & Rational::operator -= (const Rational & a) {
    return ( * this) += (-a);
}
Rational & Rational::operator /= (const Rational & a) {
    ( * this).num *= a.denom;
    ( * this).denom *= a.num;
    if (( * this).denom < 0) {
        ( * this).denom *= -1;
        ( * this).num *= -1;
    }
    reduce();
    return *this;
}

#endif