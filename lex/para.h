#pragma once

#include <iostream>
#include <fstream>
#include <memory>
#include <algorithm>
#include <vector>
#include <stack>
#include <queue>
#include <boost/unordered_set.hpp>
#include <string>

const int MAXN=32;

const boost::unordered_set<std::string> KEYWORD = {"main","cin","cout","auto","break","case","char","const","continue","default","do","double","else","enum","extern","float","for","goto","if","int","long","register","return","short","signed","sizeof","static","struct","switch","typedef","unsigned","union","void","volatile","while"};
const boost::unordered_set<std::string> SEPARATOR = {",", ";", "(", ")", "[", "]", "{", "}"};
const boost::unordered_set<std::string> OPERATOR = {"+", "-", "*", "/", "&", "^", "=", ">", "<", ">=", "<=", "==", "+=", "-=", "*=", "/="};

const char annotationSign = '#';
const char letterSign = 'l';
const char numberSign = 'z';
