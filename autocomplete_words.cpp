
#include <bits/stdc++.h>
using namespace std;
#define CYAN "\x1B[36m"
#define CLEAR "\x1B[0m"
#define RED "\x1B[31m"
#define GREEN "\x1B[32m"
#define YELLOW "\x1B[33m"
#define BLUE "\x1B[34m"
#define MAG "\x1B[35m"
#define MAX 100
#define modValue 1000000007
typedef long long int ll;

ll edit_distance(string a, string b)
{
    ll m = a.size();
    ll n = b.size();
    ll dp[m + 1][n + 1];

    for (int i = 0; i <= m; i++) //first column
        dp[i][0] = i;

    for (int j = 0; j <= n; j++) // first row
        dp[0][j] = j;            //convert a null string to some substring of b

    //dp[i][j] is the edit distance between x[0...i-1] and y[0...j-1]
    for (int i = 1; i <= m; i++)
        for (int j = 1; j <= n; j++)
        {
            int diff = 1;
            if (a[i - 1] == b[j - 1])
                diff = 0;
            dp[i][j] = min(1 + dp[i - 1][j], 1 + dp[i][j - 1]);
            dp[i][j] = min(diff + dp[i - 1][j - 1], dp[i][j]);
        }
    return dp[m][n];
}

// assumption - all words are in lowercase alphabet
// every node in the trie can have atmost 26 child node
// with respect to the recursive nature of a tree - each node in a trie points to at most 26 sub-tries

struct trienode
{
    bool is_word; //to indicate a leaf node / end of word
    trienode *child[26];
};

trienode *get_new_node()
{
    // create a new node
    trienode *temp = new trienode;
    temp->is_word = false;

    for (int i = 0; i < 26; i++)
    {
        temp->child[i] = NULL;
    }

    return temp;
}

void insert(trienode *root, string key)
{
    //insert a new word in the trie
    //start from the root node

    trienode *temp = root;

    // for every character in the word
    for (int i = 0; i < (int)key.length(); i++)
    {
        int ind = int(key[i]) - 'a';

        if (temp->child[ind] == NULL)
        {
            temp->child[ind] = get_new_node();
        }
        temp = temp->child[ind];
    }

    //end of word
    temp->is_word = true;
}

bool search(trienode *root, string key)
{
    //search a word in the trie

    if (root == NULL)
        return false;

    trienode *temp = root;

    //for every character in the word
    for (int i = 0; i < (int)key.length(); i++)
    {
        int ind = int(key[i]) - 'a';
        if (temp->child[ind] == NULL)
            return false;

        temp = temp->child[ind];
    }

    return temp->is_word;
}

void find_all_words(trienode *root, string key, vector<string> &all_words)
{
    if (root == NULL)
        return;

    trienode *temp = root;

    for (int i = 0; i < (int)key.size(); i++)
    {
        int index = int(key[i]) - 'a';
        if (temp->child[index] == NULL)
            return;

        temp = temp->child[index];
    }

    // if this prefix exist in trie then do BFS from current node to find all words

    queue<pair<trienode *, string>> q;
    q.push(make_pair(temp, key));
    string this_word;

    while (!q.empty())
    {
        temp = q.front().first;
        this_word = q.front().second;

        /*if this is end of word, then add it to the all_words(vector) */
        if (temp->is_word)
            all_words.push_back(this_word);
        q.pop();
        for (int i = 0; i < 26; i++)
        {
            if (temp->child[i] != NULL)
            {
                q.push(make_pair(temp->child[i], this_word + char(i + int('a'))));
            }
        }
    }
}

void auto_complete(trienode *root, string key)
{
    //arguments - root node of the trie, prefix to be autocompleted
    //prints all the words for which key is prefix.
    // if key is empty
    if (key.size() == 0)
        return;

    vector<string> all_words;

    find_all_words(root, key, all_words);

    if (all_words.size() == 0)
    {
        printf(RED "There are no suggestions.\n");
        return;
    }
    cout << "There are " << all_words.size() << " suggestions for this prefix " << key << '\n';
    for (int i = 0; i < (int)all_words.size(); i++)
    {
        printf(YELLOW "");
        cout << all_words[i] << endl;
        printf(CLEAR "");
    }
    return;
}

int main()
{
    /*creats root node */
    trienode *root = get_new_node();

    /*creating dictionary*/
    FILE *fp = fopen("dict.txt", "r");
    char str[25];
    while (!feof(fp))
    {
        fscanf(fp, "%s", str);
        insert(root, str);
    }
    fclose(fp);

    string key;
    while (1)
    {
        printf(BLUE "Enter key to autocomplete : ");
        cin >> key;
        auto_complete(root, key);
    }
    return 0;
}