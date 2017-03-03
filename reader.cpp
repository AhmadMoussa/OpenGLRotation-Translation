/*
 
 To reverse a vector you simply include <vector> and <algorithm>
 then you this reverse(vectorName.begin(), vectorName.end());
 
 Same thing for sorting the vector, you do sort(vectorName.begin(), vectorName.end());
 
 There is something similar for strings where you do reverse(stringName.begin(), stringName.end())
 but here no need to include algorithm : p
 
 I included a method for parsing a text file and throwing individual tokens into a vector that is returned
 
 To add stuff to the vector you call vectorName.push_back(item)
 
 There is a function that finds the longest common subsequence between two strings which is dynamic programming
 this is the link http://www.geeksforgeeks.org/printing-longest-common-subsequence/ 
 
 To convert a string to a char* you call stringName.c_str(); and for the opposite way you do charPointerName.str();
 to find the length of a char array you can strlen(charArrayName);
 
 Here is a link for the BFS algorithm http://www.geeksforgeeks.org/breadth-first-traversal-for-a-graph/
 Dunno how helpful that will be
 
 Also the miller rabin test for primality quickly snatched form the internet, it's the last three functions
 
 I hope this helps and gud luck
 *smooches* -> lav
 

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

//those three are for the miller rabin test
#include <cstring>
#include <cstdlib>
#define ll long long

using namespace std;

// Code for the longest common sequence in between two strings, you can find that online though, this be dynamic programming
Returns length of LCS for X[0..m-1], Y[0..n-1]
void lcs( char *X, char *Y, int m, int n )
{
    int L[m+1][n+1];
    
     Following steps build L[m+1][n+1] in bottom up fashion. Note
     that L[i][j] contains length of LCS of X[0..i-1] and Y[0..j-1] 
    for (int i=0; i<=m; i++)
    {
        for (int j=0; j<=n; j++)
        {
            if (i == 0 || j == 0)
                L[i][j] = 0;
            else if (X[i-1] == Y[j-1])
                L[i][j] = L[i-1][j-1] + 1;
            else
                L[i][j] = max(L[i-1][j], L[i][j-1]);
        }
    }
    
    // Following code is used to print LCS
    int index = L[m][n];
    
    // Create a character array to store the lcs string
    char lcs[index+1];
    lcs[index] = '\0'; // Set the terminating character
    
    // Start from the right-most-bottom-most corner and
    // one by one store characters in lcs[]
    int i = m, j = n;
    while (i > 0 && j > 0)
    {
        // If current character in X[] and Y are same, then
        // current character is part of LCS
        if (X[i-1] == Y[j-1])
        {
            lcs[index-1] = X[i-1]; // Put current character in result
            i--; j--; index--;     // reduce values of i, j and index
        }
        
        // If not same, then find the larger of two and
        // go in the direction of larger value
        else if (L[i-1][j] > L[i][j-1])
            i--;
        else
            j--;
    }
    
    // Print the lcs
    cout << "LCS of " << X << " and " << Y << " is " << lcs;
}

// reverse a string
string reverseString(string s){
    string copy(s);
    cout << s << '\n' << copy << '\n';
    reverse(copy.begin(), copy.end());
    return copy;
}

// removes duplicate letters from a string
string removeDuplicateLetters(string s){
    for ( string::size_type i = 0; i < s.size(); i++ )
    {
        string::size_type j = i + 1;
        while ( j < s.size() )
        {
            if ( s[i] == s[j] )
            {
                s.erase( j, 1 );
            }
            else
            {
                ++j;
            }
        }
    }
    return s;
}

//reads individual tokens from a textfile and returns a vector containing them
vector<string> parseTokens(string filePath)
{
    vector<string> tokens;
    
    ifstream file;
    file.open(filePath);
    
    if( file.is_open() ){
        string token;
        while(file >> token)
        {
            tokens.push_back(token);
        }
    }
    else
    {
        cout << "Failed to open file" << endl;
    }
    
    return tokens;
}

// Miller rabin test are the next three function, ll stand for long long, there is a define included
// at the top that does that
ll mulmod(ll a, ll b, ll mod)
{
    ll x = 0,y = a % mod;
    while (b > 0)
    {
        if (b % 2 == 1)
        {
            x = (x + y) % mod;
        }
        y = (y * 2) % mod;
        b /= 2;
    }
    return x % mod;
}
*/
/*
 
 * modular exponentiation
 
ll modulo(ll base, ll exponent, ll mod)
{
    ll x = 1;
    ll y = base;
    while (exponent > 0)
    {
        if (exponent % 2 == 1)
            x = (x * y) % mod;
        y = (y * y) % mod;
        exponent = exponent / 2;
    }
    return x % mod;
}


 * Miller-Rabin primality test, iteration signifies the accuracy
 
bool Miller(ll p,int iteration)
{
    if (p < 2)
    {
        return false;
    }
    if (p != 2 && p % 2==0)
    {
        return false;
    }
    ll s = p - 1;
    while (s % 2 == 0)
    {
        s /= 2;
    }
    for (int i = 0; i < iteration; i++)
    {
        ll a = rand() % (p - 1) + 1, temp = s;
        ll mod = modulo(a, temp, p);
        while (temp != p - 1 && mod != 1 && mod != p - 1)
        {
            mod = mulmod(mod, mod, p);
            temp *= 2;
        }
        if (mod != p - 1 && temp % 2 == 0)
        {
            return false;
        }
    }
    return true;
}

int main(){
    vector<string> tokens = parseTokens("translationCurve.txt");
    cout<< tokens[0] << endl;
    
    string s = "Blah blah...";
    string duplicatesRemoved = removeDuplicateLetters(s);
    
    cout << s << " " << duplicatesRemoved << endl;
    
    string reversed = reverseString(duplicatesRemoved);
    
    reverse(tokens.begin(), tokens.end());
    for(int i = 0; i < tokens.size(); i++){
        cout << tokens[i] << " ";
    }
    cout << endl;
    
    sort(tokens.begin(), tokens.end());
    
    for(int i = 0; i < tokens.size(); i++){
        cout << tokens[i] << " ";
    }
    cout << endl;
    
    // the first number is the number to test and the second one is the number of iterations, the more you have the more accurate the test is
    cout << Miller(1618033 , 5) << endl;
    
    
    return 0;
}

*/
