/*line encoder designed By Shivam Singh Khatri*/

/*Implemented Following encoding schemes
NRZL
NRZI
Manchester
AMI*/

/*Language Used-C++ with openGL*/

#include <iostream>
#include <string.h>
#include <algorithm>
#include <GL/glut.h>

using namespace std;

string randomData;
int encodingchoice;
int scrambling;

void NRZL(string s);
void NRZI(string s);
void Manchester(string s);
void AMI(string s);

void init()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    glPointSize(5.0);
    glBegin(GL_LINES);
    glVertex2i(1, 0);
    glVertex2i(80, 0);
    glColor3f(1.0, 1.0, 0);
    glVertex2i(1, -70);
    glVertex2i(1, 70);

    switch (encodingchoice)
    {
    case 1:
        NRZL(randomData);
        break;
    case 2:
        NRZI(randomData);
        break;
    case 3:
        Manchester(randomData);
        break;
    case 4:
        AMI(randomData);
        break;
    default:
        cout << "Wrong Choice is selected." << endl;
    }
    glEnd();
    glFlush();
}

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 100, -20, 20);
    glMatrixMode(GL_MODELVIEW);
}

string generaterandom(int strlen)
{
    string randomstring;
    int bit;
    for (int i = 0; i < strlen; i++)
    {
        bit = (int)rand() % 2;
        randomstring += to_string(bit);
    }
    return randomstring;
}

string generatefixedrandom(int strlen, int type, int bitstrlen)
{
    string randomstring, substring;
    int bit, position;
    for (int i = 0; i < strlen - bitstrlen; i++)
    {
        bit = (int)rand() % 2;
        randomstring += to_string(bit);
    }
    position = (int)rand() % (strlen - bitstrlen);

    for (int i = 0; i < bitstrlen; i++)
    {
        substring += to_string(type);
    }
    randomstring = randomstring.insert(position, substring);
    return randomstring;
}

string lcs(string &main, string &rev)
{
    int m = main.size();
    int n = rev.size();
    int dp[m + 1][n + 1];
    for (int i = 0; i <= m; i++)
    {
        for (int j = 0; j <= n; j++)
        {
            if (i == 0 || j == 0)
                dp[i][j] = 0;
            else if (main[i - 1] == rev[j - 1])
                dp[i][j] = dp[i - 1][j - 1] + 1;
            else
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
        }
    }
    int index = dp[m][n];
    string palindromestr(index + 1, '\0');
    int i = m, j = n;
    while (i > 0 && j > 0)
    {
        if (main[i - 1] == rev[j - 1])
        {
            palindromestr[index - 1] = main[i - 1];
            i--;
            j--;
            index--;
        }
        else if (dp[i - 1][j] > dp[i][j - 1])
            i--;
        else
            j--;
    }
    return palindromestr;
}

string longestpalindrome(string &mainstr)
{
    string revstr = mainstr;
    reverse(revstr.begin(), revstr.end());
    return lcs(mainstr, revstr);
}

int main(int argc, char **argv)
{
    cout << "Choose the type of sequence :" << endl
         << "1. For random sequence" << endl
         << "2. For bitstream with fixed stream:" << endl;
    int choice;
    cin >> choice;
    if (choice == 1)
    {
        int size;
        cout << "Enter the size for your bitstream:"
             << " ";
        cin >> size;
        cout << "************************************************************************************************************************" << endl;
        randomData = generaterandom(size);
    }
    if (choice == 2)
    {
        int size;
        cout << "Enter the size for your bitstream:"
             << " ";
        cin >> size;
        int bit_type, bitno;
        cout << "Enter the fixed bit type--" << endl;
        cout << "0. For zero " << endl
             << "1. for One :"
             << " ";
        cin >> bit_type;
        cout << "*************************************************************************************************************************" << endl;
        cout << "Now choose the fixed size for the bits: " << endl
             << "Press 4 for 4 fixed bit size" << endl
             << "Press 8 for 8 fixed bit size :"
             << " ";
        cin >> bitno;
        randomData = generatefixedrandom(size, bit_type, bitno);
    }
    cout << "Random Data Generated:"
         << " " << randomData << endl;
    string palindrome;
    palindrome = longestpalindrome(randomData);
    cout << "Longest palindrome in the random data is:"
         << " " << palindrome << endl;
    cout << "************************************************************************************************************************" << endl;
    cout << "Enter Line encoding scheme-\n1)NRZ-L\n2)NRZ-I\n3)MANCHESTER\n4)AMI" << endl;
    cin >>encodingchoice;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB);
    glutInitWindowPosition(200, 100);
    glutInitWindowSize(1000, 500);
    glutCreateWindow("Encoded Bitstream");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    init();
    glutMainLoop();

    return 0;
}

void NRZL(string s)
{
    int x = 1;
    int i = 0;
    for (int t = 0; t < s.length(); t++)
    {
        if (randomData[i] == '1')
        {
            glVertex2i(x, 5);
            glVertex2i(x + 5, 5);

            if (randomData[i + 1] == '0')
            {
                glVertex2i(x + 5, 5);
                glVertex2i(x + 5, -5);
            }
        }
        else if (randomData[i] == '0')
        {
            glVertex2i(x, -5);
            glVertex2i(x + 5, -5);

            if (randomData[i + 1] == '1')
            {
                glVertex2i(x + 5, 5);
                glVertex2i(x + 5, -5);
            }
        }
        i++;
        x += 5;
    }
}

void NRZI(string s)
{
    int idx = 0; 
    int x_pos = 1;
    bool ishigh = s[0] == '1' ? true : false;
    while(s[idx] != '\0')
    {
        if(s[idx] == '1')
        {
            if(ishigh)
            {
                glVertex2f(x_pos, 5.0);
                glVertex2f(x_pos+5, 5.0);
            }
            else
            {
                glVertex2f(x_pos, -5.0);
                glVertex2f(x_pos+5, -5.0);
            }

            if(s[idx+1] == '1')
            {
                glVertex2f(x_pos+5, 5.0);
                glVertex2f(x_pos+5, -5.0);
                ishigh = ishigh == true ? false : true;

            }

            x_pos += 5;
            idx ++;
        }
        else
        {  
            if(ishigh)
            {
                glVertex2f(x_pos, 5.0);
                glVertex2f(x_pos+5, 5.0);
            }
            else
            {
                glVertex2f(x_pos, -5.0);
                glVertex2f(x_pos+5, -5.0);
            }

            if(s[idx+1] == '1')
            {
                glVertex2f(x_pos+5, -5.0);
                glVertex2f(x_pos+5, 5.0);
                ishigh = ishigh == true ? false : true;
            }

            x_pos += 5;
            idx ++;
        }
    }
}


void Manchester(string s)
{
    float x = 1;
    int i = 0;
    for (int t = 0; t < s.length(); t++)
    {
        if (randomData[i] == '1')
        {
            glVertex2f(x, -5);
            glVertex2f(x + 2.5, -5);
            glVertex2f(x + 2.5, 5);
            glVertex2f(x + 2.5, -5);
            glVertex2f(x + 2.5, 5);
            glVertex2f(x + 5, 5);
            if (randomData[i + 1] == '1')
            {
                glVertex2f(x + 5, -5);
                glVertex2f(x + 5, 5);
            }
        }
        else if (randomData[i] == '0')
        {
            glVertex2f(x, 5);
            glVertex2f(x + 2.5, 5);
            glVertex2f(x + 2.5, 5);
            glVertex2f(x + 2.5, -5);
            glVertex2f(x + 2.5, -5);
            glVertex2f(x + 5, -5);

            if (randomData[i + 1] == '0')
            {
                glVertex2f(x + 5, -5);
                glVertex2f(x + 5, 5);
            }
        }
        i++;
        x += 5;
    }
}

void AMI(string s)
{
    float x = 1;
    int i = 0;
    bool isHigh = s[0] == '1' ? true : false;
    for (int t = 0; t < s.length(); t++)
    {
        if (isHigh)
        {
            if (randomData[i] == '1')
            {
                glVertex2f(x, 0);
                glVertex2f(x , 5);
                glVertex2f(x, 5);
                glVertex2f(x + 5, 5);
                glVertex2f(x + 5, 5);
                glVertex2f(x + 5, 0);

                if (randomData[i + 1] == '1')
                {
                    isHigh = isHigh == true ? false : true;
                }
            }
        }
        else
        {
            if (randomData[i] == '1')
            {
                glVertex2f(x, 0);
                glVertex2f(x, -5);
                glVertex2f(x , -5);
                glVertex2f(x + 5, -5);
                glVertex2f(x + 5, -5);
                glVertex2f(x + 5, 0);
            }
            if (randomData[i + 1] == '1')
            {
                isHigh = isHigh == true ? false : true;
            }
        }

        if (randomData[i] == '0')
        {
            glVertex2f(x, 0);
            glVertex2f(x + 5, 0);
            if (randomData[i + 1] == '1')
            {
                isHigh = isHigh == true ? false : true;
            }
        }
        i++;
        x += 5;
    }
}
