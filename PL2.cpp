# include <stdlib.h>
# include <stdio.h>
# include <string>
# include <iostream>
# include <vector>
# include <sstream>
# include <iomanip>
# include <ctype.h>
# include <cmath>
# include <cstdlib>
using namespace std;
bool gLine = false ;
bool gdot = false ;
static const string ENUM_STR[] =
{   "NUM", "IDENT", "SPECIAL", "NONE", "CONSTANT",
  "INT", "FLOAT", "CHAR",  "BOOL",  "STRING" ,  "VOID", // int , float , char , bool , string , void    
  "IF" , "ELSE" , "WHILE",  "DO",  "RETURN",  // if , else , while , do , return
  "GE", "LE", "EQ", "NEQ", "AND", "OR",  // >= , <= , == , !=, &&, ||    
  "PE", "ME", "TE", "DE", "RE", "PP", "MM", "RS", "LS",      // +=, -=, *=, /=, %=, ++, --, >>, <<
  "SLP", "SRP", "MLP", "MRP", "BLP", "BRP", 
  "ADD", "SUB", "DIV" , "MULT", "EQUAL", // + , -, /, *, =
  "SCOLON", "COM", "QUE", "COLON",
  "RIGHT", "LEFT", "REMAIN", "ONEAND", "ONEOR", "EXCLAIM","POWER",   // >, <, %, &, |, !, ^
  "ENDOFFILE"  };
const string GetTextForEnum( int EnumVal ) {

  return ENUM_STR[EnumVal] ;
} // GetTextForEnum()

enum TokenType {
  NUM, IDENT, SPECIAL, NONE, CONSTANT,
  INT, FLOAT, CHAR, BOOL, STRING, VOID, // int , float , char , bool , string , void    
  IF, ELSE, WHILE, DO, RETURN,  // if , else , while , do , return
  GE, LE, EQ, NEQ, AND, OR,  // >= , <= , == , !=, &&, ||    
  PE, ME, TE, DE, RE, PP, MM, RS, LS,      // +=, -=, *=, /=, %=, ++, --, >>, <<
  SLP, SRP, MLP, MRP, BLP, BRP,  // ( , ) , [ , ] , { , }  
  ADD, SUB, DIV, MULT, EQUAL, // + , -, /, *, =
  SCOLON, COM, QUE, COLON, // ';', ',', '?', ':'
  RIGHT, LEFT, REMAIN, ONEAND, ONEOR, EXCLAIM, POWER,   // >, <, %, &, |, !, ^
  ENDOFFILE
 
};



struct Token {
  string tokenStr;
  TokenType tokenType;
} ;

struct Assign {
  string tokenStr;
  double tokenValue;
  TokenType numType ;
  
} ;



struct VT {
  string type ;
  string ident;
  string constant ;  // 如果沒有[], 就會是空字串""
  string cbr ;
  // 有可能是 (int, float) , bool , string 三種型態
  // 全部轉string存
} ;

struct FT {
  string type;
  string ident;
  vector<Token> statements; // function 的內容
  vector<VT> reference ; // function 傳進來的東西 
  vector<VT> vList;  // function 宣告的變數 
} ;


class Scannar {

  public:
  char myNextChar ;
  string myCurToken ;
  TokenType myType ;
  vector <Token> myTotalToken ;
  int myNextLine ;
  int myCurLine ;

  bool Is_Ident( char ch ) {   // 判斷是不是字母
    return isalpha( ch ) ;
  } // Is_Ident()
 
 
  void Init() {

    myNextChar = '\0' ;
    myCurToken = "";
    myNextLine = 1;
    myType = NONE ;
    myTotalToken.clear() ;
    myCurLine = 1 ;

  } // Init()

  void IdentifierType() {

    if ( myCurToken == "int" ) myType = INT ;
    else if ( myCurToken == "char" ) myType = CHAR ;
    else if ( myCurToken == "bool" ) myType = BOOL ;
    else if ( myCurToken == "float" ) myType = FLOAT ;
    else if ( myCurToken == "string" ) myType = STRING ;
    else if ( myCurToken == "if" ) myType = IF ;
    else if ( myCurToken == "else" ) myType = ELSE ;
    else if ( myCurToken == "void" ) myType = VOID ;
    else if ( myCurToken == "while" ) myType = WHILE ;
    else if ( myCurToken == "do" ) myType = DO ;
    else if ( myCurToken == "return" ) myType = RETURN ;
    else if ( myCurToken == "true" ) myType = CONSTANT ;
    else if ( myCurToken == "false" ) myType = CONSTANT ;
    else myType = IDENT ;

  } // IdentifierType()


  int GetLine() {
    return myCurLine ;

  } // GetLine()


  void InitLine() {
    
    if ( gLine ) {
      myNextLine = 0 ;
      myCurLine = 0 ;
      gLine = false ;
    } // if
    else {
      myNextLine = 1 ;
      myCurLine = 1 ;
    } // else
  } // InitLine()

  bool Is_OtherSpecial( char ch ) {  // 判斷特殊符號

    if ( ch == '>' || ch == '<' || ch == '[' || ch == ']' || ch == '{' ||
         ch == '}' || ch == '+' || ch == '-' || ch == '%' || ch == '=' || 
         ch == '!' || ch == '(' || ch == ')' || ch == '&' || ch == ':' ||
         ch == '|' || ch == ';' || ch == '*' || ch == '/' || ch == ',' ||
         ch == '?' || ch == '^' ) {


      return true ;
    } // if
    else
      return false ;
  } // Is_OtherSpecial()


  bool Is_Num( char ch ) {  // 判斷是否為數字
    return isdigit( ch ) ;
  } // Is_Num()

  void  GetToken() {

    if ( myCurToken == "" )
      GetTotalToken() ;

    Token temToken ;
    temToken.tokenStr = myCurToken ;
    temToken.tokenType = myType ;
    myTotalToken.push_back( temToken ) ;
    // cout << myCurToken << " " << GetTextForEnum( myType ) << endl ;
    myCurToken = "" ;
    if ( myNextChar != '\0' ) {
      cin.putback( myNextChar ) ;
      if ( myNextChar == '\n' ) {
        myNextLine --  ;
      } // if 

      myNextChar = '\0' ;
    } // if

    myType = NONE ;

  } // GetToken()

  Token PeekToken() {

    if ( myCurToken == "" )
      GetTotalToken() ;

    Token temToken ;
    temToken.tokenStr = myCurToken ;
    temToken.tokenType = myType ;

    return temToken ;
  } // PeekToken()  

  void RemoveErrorLine() { // 跳過空白

    while ( myNextChar != '\n' ) {  //   white-space  
      if ( !GetNextChar() )  return;
    } // while

  } // RemoveErrorLine()

  void SkipWhiteSpace() { // 跳過空白
    while ( myNextChar == ' ' || myNextChar == '\n' || myNextChar == '\t' ) {  //   white-space  
      if ( !GetNextChar() )  return;
    } // while

  } // SkipWhiteSpace()

  void GetIdent() {

    bool ending = false ;
    while ( !ending ) {
      if ( GetNextChar() ) {
        if ( Is_Ident( myNextChar ) || myNextChar == '_' || Is_Num( myNextChar ) ) {
          myCurToken = myCurToken + myNextChar ;
        } // if
        else
          ending = true ;
      } // if
      else
        ending = true ;
    } // while
   
    IdentifierType() ; // 看有沒有型別  

  } // GetIdent()
 
 
  bool IsNoDot( string  token ) {

    for ( int i = 0 ; i < token.size() ; i++ ) {
      if ( token[i] == '.' )
        return true ;
    } // for

    return false ;

  } // IsNoDot()

  void GetNum() {

    bool ending = false ;
    bool havedot = false ;
    if ( myCurToken == "." )  // 如果一開始進來的就是 .  
      havedot = true ;

    while ( !ending ) {
      if ( GetNextChar() ) {
        if ( ( Is_Num( myNextChar ) || myNextChar == '.' ) ) {

          if ( !havedot && myNextChar == '.' ) {
            myCurToken = myCurToken + myNextChar ;
            havedot = true ;
          } // if
          else if ( havedot && myNextChar == '.' )  
            ending = true ;
          else
            myCurToken = myCurToken + myNextChar ;
        } // if          
        else
          ending = true ;
      } // if
      else
        ending = true ;

    } // while

    if ( myCurToken == "." )
      myType = NONE ;
    else
      myType = CONSTANT ;
  } // GetNum()

  void GetOtherSpecial() { 
    if ( GetNextChar() ) { 
      if ( myCurToken == "+" ) {     // ++ or += or +
        myType = ADD ;
        if ( myNextChar == '+' ) {
          myType = PP ;
          myCurToken = myCurToken + myNextChar ;
          GetNextChar() ;
        } // if
        else if ( myNextChar == '=' ) {
          myType = PE ;
          myCurToken = myCurToken + myNextChar ;
          GetNextChar() ;
        } // else if
      } // if
      else if ( myCurToken == "-" ) {    // -- or -= or - 
        myType = SUB ;
        if ( myNextChar == '-' ) {
          myType = MM ;
          myCurToken = myCurToken + myNextChar ;
          GetNextChar() ;
        } // if
        else if ( myNextChar == '=' ) {
          myType = ME ;
          myCurToken = myCurToken + myNextChar ;
          GetNextChar() ;
        } // else if
      } // else if
      else if ( myCurToken == "=" ) {     // = or ==  
        myType = EQUAL ;  
        if ( myNextChar == '=' ) {
          myType = EQ ;
          myCurToken = myCurToken + myNextChar ;
          GetNextChar() ;
        } // if
      } // else if

      else if ( myCurToken == ">" ) {   // > or >> or >=
        myType = RIGHT ;  
        if ( myNextChar == '>' ) {
          myType = RS ;
          myCurToken = myCurToken + myNextChar ;
          GetNextChar() ;
        } // if
        else if ( myNextChar == '=' ) {
          myType = GE ;
          myCurToken = myCurToken + myNextChar ;
          GetNextChar() ;
        } // else if
      } // else if	
      else if ( myCurToken == "<" ) {  // < or << or <=
        myType = LEFT ;  
        if ( myNextChar == '<' ) {
          myType = LS ;
          myCurToken = myCurToken + myNextChar ;
          GetNextChar() ;
        } // if
        else if ( myNextChar == '=' ) {
          myType = LE ;
          myCurToken = myCurToken + myNextChar ;
          GetNextChar() ;
        } // else if
      } // else if
      else if ( myCurToken == "%" ) {     // % or %=	
        myType = REMAIN ;  
        if ( myNextChar == '=' ) {
          myType = RE ;
          myCurToken = myCurToken + myNextChar ;
          GetNextChar() ;
        } // if				
      } // else if
      else if ( myCurToken == "*" ) {   // * or *=
        myType = MULT ;  
        if ( myNextChar == '=' ) {
          myType = TE ;
          myCurToken = myCurToken + myNextChar ;
          GetNextChar() ;
        } // if				
      } // else if
      else if ( myCurToken == "!" ) {  // ! or != 
        myType = EXCLAIM ;  
        if ( myNextChar == '=' ) {
          myType = NEQ ;
          myCurToken = myCurToken + myNextChar ;
          GetNextChar() ;
        } // if				
      } // else if

      else if ( myCurToken == "&" ) {  // & or && 
        myType = ONEAND ;  
        if ( myNextChar == '&' ) {
          myType = AND ;
          myCurToken = myCurToken + myNextChar ;
          GetNextChar() ;
        } // if				
      } // else if				
      else if ( myCurToken == "|" ) {  // |  or || 	
        myType = ONEOR ;  
        if ( myNextChar == '|' ) {
          myType = OR ;
          myCurToken = myCurToken + myNextChar ;
          GetNextChar() ;
        } // if				
      } // else if
      else if ( myCurToken == "/" ) {    //  / or /=
        myType = DIV ;
        if ( myNextChar == '=' ) {
          myType = DE ;
          myCurToken = myCurToken + myNextChar ;
          GetNextChar() ;  
        } // if 
      } // else if
      else if ( myCurToken == "(" )
        myType = SLP ;
      else if ( myCurToken == ")" )
        myType = SRP ;
      else if ( myCurToken == "[" )
        myType = MLP ;
      else if ( myCurToken == "]" )
        myType = MRP ;
      else if ( myCurToken == "{" )
        myType = BLP ;
      else if ( myCurToken == "}" )
        myType = BRP ;
      else if ( myCurToken == ";" )
        myType = SCOLON ;
      else if ( myCurToken == "," )
        myType = COM ;
      else if ( myCurToken == "?" )
        myType = QUE ;
      else if ( myCurToken == ":" )
        myType = COLON ;
      else if ( myCurToken == "^" )
        myType = POWER ;
      else {
        myType = NONE ;
      } // else
    } // if 

  } // GetOtherSpecial()
  
  char PeekNextChar() {

    return cin.peek() ; 
  } // PeekNextChar()

  void GetTotalToken() {      // 跳過空白以及註解 取得下一個字元 
    if ( myNextChar == '\0' )  // 取得第一個字元    
      GetNextChar() ;

    SkipWhiteSpace() ; // 跳過空白直到第一個字元  
    while ( myNextChar == '/' && PeekNextChar() == '/' ) {  // 跳過註解  
      while ( myNextChar != '\n' )
        GetNextChar() ;
      SkipWhiteSpace() ; // 跳過空白直到第一個字元
    } // while

    SkipWhiteSpace() ; // 跳過空白直到第一個字元


    CheckWhatCase() ;
  } // GetTotalToken()

  bool GetNextChar() {
    if ( cin.peek() != EOF ) {    
      myNextChar = cin.get() ;
      myCurLine = myNextLine ;
      if ( myNextChar == '\n' )
        myNextLine++ ;
      return true ;
    } // if

    return false ;

  } // GetNextChar()
  
  void GetString() {
    bool ending = false ;
    char getWhatChar = myNextChar ;
    while ( !ending ) {
      if ( GetNextChar() ) {
        if ( myNextChar != getWhatChar ) {
          myCurToken = myCurToken + myNextChar ;
        } // if
        else {
          myCurToken = myCurToken + myNextChar ; // 把最後的 " or ' 讀起來 
          GetNextChar() ;
          ending = true ;
        } // else
      } // if
      else
        ending = true ;
    } // while
    
    myType = CONSTANT ;

  }  // GetString()

  void CheckWhatCase() {
    if ( Is_Ident( myNextChar ) ) { // 第一個是字母
      myCurToken = myCurToken + myNextChar ;
      GetIdent() ;
    } // if
    else if ( Is_Num( myNextChar ) ||  myNextChar == '.' ) {  // 第一個是數字
      myCurToken = myCurToken + myNextChar ;
      GetNum() ;
    } // else if

    else if ( Is_OtherSpecial( myNextChar ) ) { // 第一個是特殊符號
      myCurToken = myCurToken + myNextChar ;
      GetOtherSpecial() ;
    } // else if
    else if ( myNextChar == '"' || myNextChar == '\'' ) {
      myCurToken = myCurToken + myNextChar ;
      GetString() ;
    } // else if
    else {
      myCurToken = myCurToken + myNextChar ;
      myType = NONE ;
    } // else

  } // CheckWhatCase()
 

  string FloatToString( float Float ) {

    stringstream str;
    str << Float;
    string s( str.str() );

    return s;

  } // FloatToString()


}; // Scannar



class Parser{

  public:
  Scannar mscanner ;
  void Init() ;
  vector <Token> myGetToken ;
  TokenType  mtokenType;
  double mtokenValue ;
  string mtokenStr ;
  vector <Assign> mAssignIdent ;
  vector<VT> myVariableList;
  vector<FT> myFunctionList;
  FT mTempFunctionList ;
  VT mTempVariableList ;
  bool mIs_Declaration;
  bool mhaveF ;


  void ListAllVariables() {
    
    myVariableList = MergeSort( myVariableList ) ;
    for ( int i = 0 ; i < myVariableList.size() ; i++ ) {
      cout << myVariableList[i].ident << endl ;
    } // for

  } // ListAllVariables()

  void ListAllFunctions() {
    
    myFunctionList = MergeSortF( myFunctionList ) ;
    for ( int i = 0 ; i < myFunctionList.size() ; i++ ) {
      cout << myFunctionList[i].ident << "()" << endl ;
    } // for
  } // ListAllFunctions()


  void ListVariable(  string VariableConstant ) {
      // 去掉雙引號
    string str = VariableConstant ;
    str.erase( str.begin() ) ;
    str.erase( str.size()-1 ) ;
    for ( int i = 0 ; i < myVariableList.size() ; i++ ) {
      if ( str == myVariableList[i].ident ) {
        cout << myVariableList[i].type << " " << myVariableList[i].ident ;
        if ( myVariableList[i].constant != "" )
          cout << "[ " << myVariableList[i].constant << " ]"  ; 
        cout << " ;" << endl ;
        return ;
      } // if
    } // for
  } // ListVariable()


  void ListFunction( string FunctionConstant ) {
    int space = 0 ;
    string str = "" ;
    str = FunctionConstant.substr( 1, FunctionConstant.size()-2 ) ;
    vector <Token> statement ;
    for ( int i = 0 ; i < myFunctionList.size() ; i++ ) {
      if ( str == myFunctionList[i].ident ) {
        statement = myFunctionList[i].statements;
        cout << myFunctionList[i].type << " " << myFunctionList[i].ident << "( " ;
        for ( int j = 0 ; j < myFunctionList[i].reference.size() ; j++ ) {
          cout << myFunctionList[i].reference[j].type << " " ;
          if ( myFunctionList[i].reference[j].cbr != "" )
            cout << "& "  ;
          cout << myFunctionList[i].reference[j].ident ;
          if ( myFunctionList[i].reference[j].constant != "" )
            cout << "[ " << myFunctionList[i].reference[j].constant << " ]" ;
          if ( j != myFunctionList[i].reference.size() - 1 )
            cout << ", " ;
        } // for
  
        cout << " ) "  ;
        for ( int k = 0 ; k < statement.size() ; k++ ) {

          if ( statement[k].tokenType == BLP ) 
            space += 2 ;
          if ( k != statement.size()-1 && statement[k+1].tokenType == BRP  )
            space -= 2 ;
          if ( statement[k].tokenType == BLP || statement[k].tokenType == BRP 
               || statement[k].tokenType == SCOLON  ) {
            cout << statement[k].tokenStr ;
            cout << endl ;
            for ( int a = 0 ; a < space ; a++ )
              cout << " " ;
          } // if
          else {
            cout << statement[k].tokenStr ;
            if ( ( statement[k].tokenType == IDENT  && statement[k+1].tokenType == SLP )  || 
                 ( statement[k].tokenType == IDENT && statement[k+1].tokenType  == MLP ) ||
                 ( statement[k].tokenType == IDENT && statement[k+1].tokenType == COM ) ||
                 ( statement[k].tokenType == IDENT && ( statement[k+1].tokenType == PP || 
                                                        statement[k+1].tokenType == MM ) ) ||
                 ( statement[k].tokenType == SLP && statement[k+1].tokenType == SRP ) ) 
              ;
            else
              cout << " " ;    
          } // else  
        } // for
      } // if 
    } // for
  } // ListFunction()


  bool CheckIDisAssign( string tempIdstr ) {

    for ( int i = 0 ; i < myVariableList.size() ; i++ ) {
      if ( tempIdstr == myVariableList[i].ident ) {
        return true ;
      } // if
    } // for

    for ( int i = 0 ; i < myFunctionList.size() ; i++ ) {
      if ( tempIdstr == myFunctionList[i].ident ) {
        return true ;
      } // if
    } // for

    if ( tempIdstr == mTempFunctionList.ident )
      return true ;

    if ( tempIdstr == "cin" || tempIdstr == "cout" )
      return true ;

    if ( mhaveF ) {
      for ( int i = 0 ; i < mTempFunctionList.vList.size() ; i++ ) {
        if (  tempIdstr == mTempFunctionList.vList[i].ident )
          return true ;
      } // for

      for ( int i = 0 ; i < mTempFunctionList.reference.size() ; i++ ) {
        if ( tempIdstr == mTempFunctionList.reference[i].ident ) {
          return true ;
        } // if
      } // for
    } // if 

    UndefinedToken( mtokenStr ) ;
    return false ;

  } // CheckIDisAssign()

  bool DoSomeThing() {  
    string tempConstant = "" ;
    PeekToken( mtokenType, mtokenStr ) ;
    if ( mtokenStr == "ListAllVariables" ) {
      GetToken( mtokenType, mtokenStr ) ;
      PeekToken( mtokenType, mtokenStr ) ;
      if ( mtokenStr == "(" ) { 
        GetToken( mtokenType, mtokenStr ) ;
        PeekToken( mtokenType, mtokenStr ) ;
        if ( mtokenStr == ")" ) {
          GetToken( mtokenType, mtokenStr ) ;
          PeekToken( mtokenType, mtokenStr ) ;
          if ( mtokenStr == ";" ) {
            GetToken( mtokenType, mtokenStr ) ;
            ListAllVariables() ;
            return true ;
          } // if
        } // if
      } // if
    } // if
    else if ( mtokenStr == "ListAllFunctions" ) {
      GetToken( mtokenType, mtokenStr ) ;
      PeekToken( mtokenType, mtokenStr ) ;
      if ( mtokenStr == "(" ) { 
        GetToken( mtokenType, mtokenStr ) ;
        PeekToken( mtokenType, mtokenStr ) ;
        if ( mtokenStr == ")" ) {
          GetToken( mtokenType, mtokenStr ) ;
          PeekToken( mtokenType, mtokenStr ) ;
          if ( mtokenStr == ";" ) {
            GetToken( mtokenType, mtokenStr ) ;
            ListAllFunctions() ;
            return true ;
          } // if
        } // if
      } // if
    } // else if
    else if ( mtokenStr == "ListVariable" ) {
      GetToken( mtokenType, mtokenStr ) ;
      PeekToken( mtokenType, mtokenStr ) ;
      if ( mtokenStr == "(" ) { 
        GetToken( mtokenType, mtokenStr ) ;
        PeekToken( mtokenType, mtokenStr ) ;
        if ( mtokenType == CONSTANT ) {
          GetToken( mtokenType, mtokenStr ) ;
          tempConstant = mtokenStr ;
          PeekToken( mtokenType, mtokenStr ) ;
          if ( mtokenStr == ")" ) {
            GetToken( mtokenType, mtokenStr ) ;
            PeekToken( mtokenType, mtokenStr ) ;
            if ( mtokenStr == ";" ) {
              GetToken( mtokenType, mtokenStr ) ;
              ListVariable( tempConstant );
              return true ;
            } // if
          } // if
        } // if
      } // if
    } // else if
    else if ( mtokenStr == "ListFunction" ) {
      GetToken( mtokenType, mtokenStr ) ;
      PeekToken( mtokenType, mtokenStr ) ;
      if ( mtokenStr == "(" ) { 
        GetToken( mtokenType, mtokenStr ) ;
        PeekToken( mtokenType, mtokenStr ) ;
        if ( mtokenType == CONSTANT ) {
          GetToken( mtokenType, mtokenStr ) ;
          tempConstant = mtokenStr ;
          PeekToken( mtokenType, mtokenStr ) ;
          if ( mtokenStr == ")" ) {
            GetToken( mtokenType, mtokenStr ) ;
            PeekToken( mtokenType, mtokenStr ) ;
            if ( mtokenStr == ";" ) {
              GetToken( mtokenType, mtokenStr ) ;
              ListFunction( tempConstant );
              return true ;
            } // if
          } // if
        } // if
      } // if
    } // else if 
    else if ( mtokenStr == "Done" ) {
      GetToken( mtokenType, mtokenStr ) ;
      return false ;
    } // else if

    return false ;

  } // DoSomeThing()

  bool User_input() {   
  // User_input : ( definition | statement ) { definition | statement }
    Init() ;
    vector<Token> tempVector;
    bool finish = false ;
    mscanner.InitLine() ;
    PeekToken( mtokenType, mtokenStr ) ;
    
    if ( DoSomeThing() ) { // 做四大 function  
      cout << "Statement executed ..." << endl ;
      return true ;
    } // if
    else {
      if ( mtokenStr == "Done" )
        return false ;
    } // else

    if ( DefinitionHead( mtokenType ) ) {
      if ( Definition() ) {
        gLine = true ;
        return true ;
      } // if
      else { 
        GetToken( mtokenType, mtokenStr ) ;
        mscanner.RemoveErrorLine() ;
        mscanner.myTotalToken.clear() ;
        return true ;
      } // else
    } // if
    else if ( StatementHead( mtokenType ) ) {
      if ( Statement() ) {
        cout << "Statement executed ..." << endl ;
        gLine = true ;
        return true ;
      } // if
      else { 
        GetToken( mtokenType, mtokenStr ) ;
        mscanner.RemoveErrorLine() ;
        return true ;
      } // else
    } // else if
    else { 
      UnexpectedToken( mtokenStr ) ;
      GetToken( mtokenType, mtokenStr ) ;
      mscanner.RemoveErrorLine() ;
      return true ;
    } // else
  } // User_input()

  bool DefinitionHead( TokenType mtokenType ) {
    if ( mtokenType == VOID || mtokenType == INT || mtokenType == CHAR ||
         mtokenType == FLOAT || mtokenType == STRING || mtokenType == BOOL ) {

      // GetToken( mtokenType, mtokenStr ) ; // 下一個字元 
      return true ;
    } // if

    return false ;

  } // DefinitionHead()


  bool StatementHead( TokenType mtokenType ) {

    if ( mtokenType == SCOLON || mtokenType == IDENT || mtokenType == PP ||
         mtokenType == MM || mtokenType == ADD || mtokenType == SUB ||
         mtokenType == EXCLAIM || mtokenType == CONSTANT || mtokenType == SLP ||
         mtokenType == RETURN || mtokenType == BLP || mtokenType == IF || 
         mtokenType == WHILE || mtokenType == DO ) {

      // GetToken( mtokenType, mtokenStr ) ; // 下一個字元  

      return true ;
    } // if

    return false ;

  } // StatementHead()

  void GetFunctionInformation() {
    vector<Token> tempFunctionInformation = mscanner.myTotalToken;
    // Formal_parameter_list_Information( TempFunctionInformation ) ;
    while ( tempFunctionInformation[0].tokenStr != "{" ) {
      // cout << TempFunctionInformation[0].tokenStr << endl ;
      tempFunctionInformation.erase( tempFunctionInformation.begin() );
    } // while

    mTempFunctionList.statements = tempFunctionInformation ;
    mscanner.myTotalToken.clear() ;
  } // GetFunctionInformation()
  
  vector<VT> MergeSort( vector<VT> VList ) {

    if ( VList.size() > 1 ) {
      int mid = VList.size() / 2 ;
      vector<VT> lefthalf( VList.begin(), VList.begin() + mid ) ;
      vector<VT> righthalf( VList.begin() + mid, VList.begin() + VList.size() ) ;

      lefthalf = MergeSort( lefthalf );
      righthalf = MergeSort( righthalf );

      int i = 0;
      int j = 0;
      int k = 0;
      while ( i < lefthalf.size() && j < righthalf.size() ) {
        if ( lefthalf[i].ident < righthalf[j].ident ) {
          VList[k] = lefthalf[i] ;
          i++;
        } // if
        else {
          VList[k] = righthalf[j] ;
          j++;
        } // else
            
        k++;
      } // while

      while ( i < lefthalf.size() ) {
        VList[k] = lefthalf[i] ;
        i++;
        k++;
      } // while 

      while ( j < righthalf.size() ) {
        VList[k] = righthalf[j] ;
        j++;
        k++;
      } // while
    } // if
   
    return VList ;
  } // MergeSort()
  
  
  vector<FT> MergeSortF( vector<FT> FList ) {

    if ( FList.size() > 1 ) {
      int mid = FList.size() / 2 ;
      vector<FT> lefthalf( FList.begin(), FList.begin() + mid ) ;
      vector<FT> righthalf( FList.begin() + mid, FList.begin() + FList.size() ) ;

      lefthalf = MergeSortF( lefthalf );
      righthalf = MergeSortF( righthalf );

      int i = 0;
      int j = 0;
      int k = 0;
      while ( i < lefthalf.size() && j < righthalf.size() ) {
        if ( lefthalf[i].ident < righthalf[j].ident ) {
          FList[k] = lefthalf[i] ;
          i++;
        } // if
        else {
          FList[k] = righthalf[j] ;
          j++;
        } // else
            
        k++;
      } // while

      while ( i < lefthalf.size() ) {
        FList[k] = lefthalf[i] ;
        i++;
        k++;
      } // while 

      while ( j < righthalf.size() ) {
        FList[k] = righthalf[j] ;
        j++;
        k++;
      } // while
    } // if
   
    return FList ;
  } // MergeSortF()
  
  void GetFunction() {
    for ( int i = 0 ; i < myFunctionList.size() ; i++ ) {
      if ( mTempFunctionList.ident == myFunctionList[i].ident ) {
        myFunctionList[i] = mTempFunctionList ;
        cout << "New definition of " << mTempFunctionList.ident << "()" <<  " entered ..." << endl ;
        return ;
      } // if
    } // for

    cout << "Definition of " << mTempFunctionList.ident << "()" <<  " entered ..." << endl ;
    myFunctionList.push_back( mTempFunctionList ) ;
    // if ( myFunctionList.size() >= 2  )
      // BubbleSortF( myFunctionList ) ;
  } // GetFunction()
  

  void GetVariable( VT TempList ) {
    if ( mIs_Declaration ) {
      for ( int i = 0 ; i < mTempFunctionList.vList.size() ; i++ ) {
        if ( TempList.ident == mTempFunctionList.vList[i].ident ) {
          mTempFunctionList.vList[i] = TempList ;
          return ;
        } // if
      } // for

      mTempFunctionList.vList.push_back( TempList ) ;
      
    } // if
    else {

      for ( int i = 0 ; i < myVariableList.size() ; i++ ) {
        if ( TempList.ident == myVariableList[i].ident ) {
          myVariableList[i] = TempList ;
          cout << "New definition of " << TempList.ident <<  " entered ..." << endl ;
          return; 
        } // if
      } // for

      cout << "Definition of " << TempList.ident  <<  " entered ..." << endl ;
      myVariableList.push_back( TempList ) ;
      // if ( myVariableList.size() >= 2   )
        // myVariableList( myVariableList ) ;
    } // else

    mIs_Declaration = false ;
  } // GetVariable()
  
  bool Definition() {
      //   :  VOID Identifier function_definition_without_ID 
      //    | type_specifier Identifier function_definition_or_declarators
    string tempType = "";
    string tempName = "";
    PeekToken( mtokenType, mtokenStr ) ;
    if ( mtokenType == VOID ) {     // 判斷是否為VOID  
      GetToken( mtokenType, mtokenStr ) ;  // 取得VOID  
      mTempFunctionList.type = mtokenStr ;
      PeekToken( mtokenType, mtokenStr ) ; // 下一個字元 
      if ( mtokenType == IDENT ) {
        GetToken( mtokenType, mtokenStr ) ;
        mTempFunctionList.ident = mtokenStr ;
        mhaveF = true ;
        if ( Function_definition_without_ID() ) { // CALL function_definition_without_ID
          GetFunctionInformation() ;  // 將function 的 內容都儲存起來 
          GetFunction() ; // 將function 放在我們list裡面 
          return true ;
        } // if 
        else return false ;
      } // if
      else {
        UnexpectedToken( mtokenStr ) ;
        return false ;
      } // else 
    } // if
    else if ( Type_specifier() ) {  // 判斷是否為 INT | CHAR | FLOAT | STRING | BOOL
      tempType = mtokenStr ;
      PeekToken( mtokenType, mtokenStr ) ; // 看下一個是否為 IDENT
      if ( mtokenType == IDENT ) {
        GetToken( mtokenType, mtokenStr ) ; // 看下一個是否為 IDENT 
        tempName = mtokenStr ;
        if ( Function_definition_or_declarators( tempType, tempName ) ) { 
          if ( mhaveF ) {
            GetFunctionInformation() ;  // 將function 的內容都儲存起來 
            GetFunction() ; // 將function 放在我們list裡面 
            mhaveF = false ;
          } // if
          
          return true ;

        } // if 
        else return false ;
      } // if
      else {
        UnexpectedToken( mtokenStr ) ;
        return false ;
      } // else 
    } // else if 
    else {
      UnexpectedToken( mtokenStr ) ;
      return false ;
    } // else 

  } // Definition()


  bool Type_specifier() {

   //  : INT | CHAR | FLOAT | STRING | BOOL
    PeekToken( mtokenType, mtokenStr ) ;
    if (  mtokenType == INT || mtokenType == CHAR || mtokenType == FLOAT 
         || mtokenType == STRING || mtokenType == BOOL ) {
      GetToken( mtokenType, mtokenStr ) ;
      return true ;

    } // if
    else {
      UnexpectedToken( mtokenStr ) ;
      return false ;
    } // else 

  } // Type_specifier()

  bool Function_definition_or_declarators( string temptype, string tempName ) {


    // : function_definition_without_ID | rest_of_declarators
    PeekToken( mtokenType, mtokenStr ) ; 

    if ( mtokenType == SLP ) { // 代表他是一個 function  
      mhaveF = true ;
      mTempFunctionList.type = temptype ;
      mTempFunctionList.ident = tempName ;
      if ( Function_definition_without_ID() ) {
        // cout << temptype << tempName << haveF << endl ;
        return true ;
      } // if
    } // if
    else if ( mtokenType == MLP || mtokenType == COM || mtokenType == SCOLON ) { 
      // 代表他是一個 function  一般的宣告變數 
      if ( Rest_of_declarators( temptype, tempName ) )
        return true ; 
    } // else if
    else {
      UnexpectedToken( mtokenStr ) ;
      return false ;
    } // else 

    return false ;
  } // Function_definition_or_declarators()

  bool Rest_of_declarators( string temptype, string tempName ) {

      // : [ '[' Constant ']' ] 
      // { ',' Identifier [ '[' Constant ']' ] } ';'
    vector< VT > tempList;  // 用來存','分隔的ID, 等讀完才能一次存起來 
    VT tempVariable ;
    tempVariable.type = temptype ;
    tempVariable.ident = tempName ;
    PeekToken( mtokenType, mtokenStr ) ;
    if ( mtokenType == MLP ) {
      GetToken( mtokenType, mtokenStr ) ;
      PeekToken( mtokenType, mtokenStr ) ;  // 一定要是Constant 否則 錯誤
      if ( mtokenType == CONSTANT ) {
        GetToken( mtokenType, mtokenStr ) ;
        tempVariable.constant = mtokenStr ;
        PeekToken( mtokenType, mtokenStr ) ; // 一定要是 ] 否則 錯誤
        if ( mtokenType == MRP ) {
          GetToken( mtokenType, mtokenStr ) ;
          PeekToken( mtokenType, mtokenStr ) ;  //  看下一個是不是 , 式的話可以重複 0到 n 次		
        } // if
        else {
          UnexpectedToken( mtokenStr ) ;
          return false ;
        } // else  
      } // if
      else {  // // 不是Constant錯誤
        UnexpectedToken( mtokenStr ) ;
        return false ;
      } // else  	
    } // if

    tempList.push_back( tempVariable ) ;  // 把宣告的ID存進去 

    while ( mtokenType == COM ) {
      VT tempVariable1 ;
      tempVariable1.type = temptype ;
      GetToken( mtokenType, mtokenStr ) ;
      PeekToken( mtokenType, mtokenStr ) ;
      if ( mtokenType == IDENT ) {
        GetToken( mtokenType, mtokenStr ) ;
        tempVariable1.ident = mtokenStr ;
        PeekToken( mtokenType, mtokenStr ) ; //  看下一個是不是 [ 或 , 也可以不要  
        if ( mtokenType == MLP ) {
          GetToken( mtokenType, mtokenStr ) ;
          PeekToken( mtokenType, mtokenStr ) ; // 一定要是Constant 否則 錯誤
          if ( mtokenType == CONSTANT ) {
            GetToken( mtokenType, mtokenStr ) ;
            tempVariable1.constant = mtokenStr ;
            PeekToken( mtokenType, mtokenStr ) ; // 一定要是 ] 否則 錯誤
            if ( mtokenType == MRP ) {
              GetToken( mtokenType, mtokenStr ) ;
              PeekToken( mtokenType, mtokenStr ) ; //  看下一個是不是 , 式的話可以重複 0到 n 次		
            } // if
            else {
              UnexpectedToken( mtokenStr ) ;
              return false ;
            } // else  
          } // if
          else {  // // 不是Constant錯誤
            return false ;
          } // else  	
        } // if

        tempList.push_back( tempVariable1 ) ;  // 把 , 後宣告的變數存存起來 
      } // if
      else {
        UnexpectedToken( mtokenStr ) ;
        return false ;
      } // else
      
      PeekToken( mtokenType, mtokenStr ) ;

    } // while

    if ( mtokenType == SCOLON  ) {
      GetToken( mtokenType, mtokenStr ) ;
      for ( int i = 0 ; i < tempList.size() ; i++ )
        GetVariable( tempList[i] );
      tempList.clear();  // 存過的就清掉
      mIs_Declaration = false ;
      return true ;
    } // if
    else {
      UnexpectedToken( mtokenStr ) ;
      return false ;
    } // else

    return true ;
  } // Rest_of_declarators()

  bool Function_definition_without_ID() {
    // : '(' [ VOID | formal_parameter_list ] ')' compound_statement

    PeekToken( mtokenType, mtokenStr ) ;
    if ( mtokenType == SLP ) {
      GetToken( mtokenType, mtokenStr ) ;
      PeekToken( mtokenType, mtokenStr ) ;
      if ( mtokenType == VOID ) {
        GetToken( mtokenType, mtokenStr ) ;
        PeekToken( mtokenType, mtokenStr ) ;
      } // if
      else if ( mtokenType == INT || mtokenType == CHAR || mtokenType == FLOAT ||
                mtokenType == STRING || mtokenType == BOOL ) {
        if ( Formal_parameter_list() )
          PeekToken( mtokenType, mtokenStr ) ; 
      } // else if

      if ( mtokenType == SRP ) {
        GetToken( mtokenType, mtokenStr ) ;
        if ( Compound_statement() )
          return true ;
        else {
          return false ;
        } // else
      } // if
      else {
        UnexpectedToken( mtokenStr ) ;
        return false ;
      } // else
    } // if 
    else {
      UnexpectedToken( mtokenStr ) ;
      return false ;
    } // else

  } // Function_definition_without_ID()


  bool Formal_parameter_list() {

    // : type_specifier [ '&' ] Identifier [ '[' Constant ']' ] 
    //  { ',' type_specifier [ '&' ] Identifier [ '[' Constant ']' ] }
    vector< VT > tempList ;
    VT tempreference ;

    if (  mtokenType == INT || mtokenType == CHAR || mtokenType == FLOAT 
         || mtokenType == STRING || mtokenType == BOOL ) {
      if ( !Type_specifier() )
        return false ;
      tempreference.type = mtokenStr ;
      PeekToken( mtokenType, mtokenStr ) ; // 看下一個是不是 & 也可以不要
      if ( mtokenType == ONEAND ) {
        GetToken( mtokenType, mtokenStr ) ;
        tempreference.cbr = mtokenStr ;
        PeekToken( mtokenType, mtokenStr ) ;
      } // if

      if ( mtokenType == IDENT ) {
        GetToken( mtokenType, mtokenStr ) ;
        tempreference.ident = mtokenStr ; 
        PeekToken( mtokenType, mtokenStr ) ; //  看下一個是不是 [ 或 , 也可以不要  
        if ( mtokenType == MLP ) {
          GetToken( mtokenType, mtokenStr ) ;
          PeekToken( mtokenType, mtokenStr ) ; // 一定要是Constant 否則 錯誤
          if ( mtokenType == CONSTANT ) {
            GetToken( mtokenType, mtokenStr ) ;
            tempreference.constant = mtokenStr ;
            PeekToken( mtokenType, mtokenStr ) ; // 一定要是 ] 否則 錯誤
            if ( mtokenType == MRP ) {
              GetToken( mtokenType, mtokenStr ) ;
              PeekToken( mtokenType, mtokenStr ) ; //  看下一個是不是 , 式的話可以重複 0到 n 次		
            } // if
            else {
              UnexpectedToken( mtokenStr ) ;
              return false ;
            } // else  
          } // if
          else {  // // 不是Constant錯誤
            UnexpectedToken( mtokenStr ) ;
            return false ;
          } // else  
        } // if

        tempList.push_back( tempreference ) ;

        while ( mtokenType == COM )  {    
          VT tempreference1 ;   
          GetToken( mtokenType, mtokenStr ) ;
          PeekToken( mtokenType, mtokenStr ) ;
          if ( Type_specifier() ) {
            tempreference1.type = mtokenStr ;
            PeekToken( mtokenType, mtokenStr ) ; // 看下一個是不是 & 也可以不要
            if ( mtokenType == ONEAND ) {
              GetToken( mtokenType, mtokenStr ) ;
              tempreference1.cbr = mtokenStr ;
              PeekToken( mtokenType, mtokenStr ) ;
            } // if

            if ( mtokenType == IDENT ) {
              GetToken( mtokenType, mtokenStr ) ;
              tempreference1.ident = mtokenStr ;
              PeekToken( mtokenType, mtokenStr ) ; //  看下一個是不是 [ 或 , 也可以不要  
              if ( mtokenType == MLP ) {
                GetToken( mtokenType, mtokenStr ) ;
                PeekToken( mtokenType, mtokenStr ) ; // 一定要是Constant 否則 錯誤
                if ( mtokenType == CONSTANT ) {
                  GetToken( mtokenType, mtokenStr ) ;
                  tempreference1.constant = mtokenStr ;
                  PeekToken( mtokenType, mtokenStr ) ; // 一定要是 ] 否則 錯誤
                  if ( mtokenType == MRP ) {
                    GetToken( mtokenType, mtokenStr ) ;
                    PeekToken( mtokenType, mtokenStr ) ; //  看下一個是不是 , 式的話可以重複 0到 n 次		
                  } // if
                  else {
                    UnexpectedToken( mtokenStr ) ;
                    return false ;
                  } // else  
                } // if
                else {  // // 不是Constant錯誤
                  UnexpectedToken( mtokenStr ) ;
                  return false ;
                } // else  
              } // if

              tempList.push_back( tempreference1 ) ;
            } // if
            else {
              UnexpectedToken( mtokenStr ) ;
              return false ;
            } // else 
          } // if
          else {
            UnexpectedToken( mtokenStr ) ;
            return false ;
          } // else 
        } // while

        mTempFunctionList.reference.assign( tempList.begin(), tempList.end() );
        tempList.clear() ;
        return true ;
      } // if
      else {
        UnexpectedToken( mtokenStr ) ;
        return false ;
      } // else 
    } // if
    else {
      UnexpectedToken( mtokenStr ) ;
      return false ;
    } // else 



  } // Formal_parameter_list()

  bool Compound_statement() {
      //  : '{' { declaration | statement } '}'
    PeekToken( mtokenType, mtokenStr ) ;
    if ( mtokenType == BLP ) {
      GetToken( mtokenType, mtokenStr ) ;
      if ( !mhaveF )
        mhaveF = true ;
      PeekToken( mtokenType, mtokenStr ) ;
    } // if 
    else {
      UnexpectedToken( mtokenStr ) ;
      return false ;
    } // else 

    while ( mtokenType == INT || mtokenType == CHAR || mtokenType == FLOAT ||
            mtokenType == STRING || mtokenType == BOOL || StatementHead( mtokenType ) )  {

      if ( mtokenType == INT || mtokenType == CHAR || mtokenType == FLOAT ||
           mtokenType == STRING || mtokenType == BOOL ) {
        if ( !Declaration() ) {
          return false ;
        } // if
      } // if

      else if ( StatementHead( mtokenType ) ) {
        if ( !Statement() ) {
          return false ;
        } // if
      } // else if

      PeekToken( mtokenType, mtokenStr ) ;
    } // while 

    if ( mtokenType == BRP ) {
      GetToken( mtokenType, mtokenStr ) ;
      return true ;
    } // if 
    else {
      UnexpectedToken( mtokenStr ) ;
      return false ;
    } // else 
  } // Compound_statement()


  bool Declaration() {
      //  : type_specifier Identifier rest_of_declarators
    string tempeType = "";
    string tempName = "";
    PeekToken( mtokenType, mtokenStr ) ;
    if (  mtokenType == INT || mtokenType == CHAR || mtokenType == FLOAT 
         || mtokenType == STRING || mtokenType == BOOL ) {
      if ( !Type_specifier() )
        return false ;
      tempeType = mtokenStr ;
      PeekToken( mtokenType, mtokenStr ) ;
      if ( mtokenType == IDENT ) {
        GetToken( mtokenType, mtokenStr ) ;
        tempName = mtokenStr ;
        mIs_Declaration = true ;
        if ( mtokenType == MLP || mtokenType == COM || mtokenType == SCOLON ) {
          if ( Rest_of_declarators( tempeType, tempName ) )
            return true ;
          else  return false ; 
        } // if 
        else {
          UnexpectedToken( mtokenStr ) ;
          return false ;
        } // else 
      } // if
      else {
        UnexpectedToken( mtokenStr ) ;
        return false ;
      } // else 
    } // if
    else {
      UnexpectedToken( mtokenStr ) ;
      return false ;
    } // else 	   
  } // Declaration()


  bool Statement() {
  //  : ';'     // the null statement
  //   | expression ';'  /* expression here should not be empty */
  //  | RETURN [ expression ] ';'
  //  | compound_statement
  //  | IF '(' expression ')' statement [ ELSE statement ]
  //  | WHILE '(' expression ')' statement
  //  | DO statement WHILE '(' expression ')' ';'
  
           
    PeekToken( mtokenType, mtokenStr ) ;
    if ( mtokenType == SCOLON ) {
      GetToken( mtokenType, mtokenStr ) ;
      return true ;
    } // if 
    else if ( mtokenType == IDENT || mtokenType == PP || mtokenType == MM ||
              mtokenType == ADD || mtokenType == SUB || mtokenType == EXCLAIM ||
              mtokenType == CONSTANT || mtokenType == SLP  ) {
      if ( !Expression() )
        return false ;
      PeekToken( mtokenType, mtokenStr ) ;
      if ( mtokenType == SCOLON ) {
        GetToken( mtokenType, mtokenStr ) ;
        return true ;
      } // if 
      else {
        UnexpectedToken( mtokenStr ) ;
        return false ;
      } // else
    } // else if
    else if (  mtokenType == RETURN ) {
      GetToken( mtokenType, mtokenStr ) ;
      PeekToken( mtokenType, mtokenStr ) ;
      if ( mtokenType == IDENT || mtokenType == PP || mtokenType == MM ||
           mtokenType == ADD || mtokenType == SUB || mtokenType == EXCLAIM ||
           mtokenType == CONSTANT || mtokenType == SLP ) {
        if ( !Expression() )
          return false ;
        PeekToken( mtokenType, mtokenStr ) ;
        if ( mtokenType == SCOLON ) {
          GetToken( mtokenType, mtokenStr ) ;
          return true ;
        } // if 
        else {
          UnexpectedToken( mtokenStr ) ;
          return false ;
        } // else
      } // if
      else {    // [ expression ] 沒有進去 
        PeekToken( mtokenType, mtokenStr ) ;
        if ( mtokenType == SCOLON ) {
          GetToken( mtokenType, mtokenStr ) ;
          return true ;
        } // if
        else {
          UnexpectedToken( mtokenStr ) ;
          return false ;
        } // else
      } // else 
    } // else if

    else if ( mtokenType == BLP ) {
      if ( !Compound_statement() )
        return false ;
      return true ;
    } // else if

    //  | IF '(' expression ')' statement [ ELSE statement ]
    else if ( mtokenType == IF ) {
      GetToken( mtokenType, mtokenStr ) ;
      PeekToken( mtokenType, mtokenStr ) ;
      if ( mtokenType == SLP  ) {
        GetToken( mtokenType, mtokenStr ) ;
        if ( Expression() ) {
          PeekToken( mtokenType, mtokenStr ) ;
          if ( mtokenType == SRP ) {
            GetToken( mtokenType, mtokenStr ) ;
            if ( Statement() ) {
              PeekToken( mtokenType, mtokenStr ) ;
              if ( mtokenType == ELSE ) {
                GetToken( mtokenType, mtokenStr ) ;
                if ( Statement()  ) {
                  return true ;
                } // if
                else return false ;
              } // if
            } // if
            else return false ;
          } // if
          else {
            UnexpectedToken( mtokenStr ) ;
            return false ;
          } // else	
        } // if
        else return false ;
      } // if
      else {
        UnexpectedToken( mtokenStr ) ;
        return false ;
      } // else	 
    } // else if

    //  | WHILE '(' expression ')' statement	
    else if ( mtokenType == WHILE ) {
      GetToken( mtokenType, mtokenStr ) ;
      PeekToken( mtokenType, mtokenStr ) ;
      if ( mtokenType == SLP  ) {
        GetToken( mtokenType, mtokenStr ) ;
        if ( Expression() ) {
          PeekToken( mtokenType, mtokenStr ) ;
          if ( mtokenType == SRP ) {
            GetToken( mtokenType, mtokenStr ) ;
            if ( Statement() ) {
              return true ;
            } // if 
            else return false ;
          } // if
          else {
            UnexpectedToken( mtokenStr ) ;
            return false ;
          } // else	
        } // if
        else return false ;

      } // if
      else {
        UnexpectedToken( mtokenStr ) ;
        return false ;
      } // else			
    } // else if

    //  | DO statement WHILE '(' expression ')' ';'

    else if ( mtokenType == DO ) {
      GetToken( mtokenType, mtokenStr ) ;
      if ( Statement() ) {
        PeekToken( mtokenType, mtokenStr ) ;
        if ( mtokenType == WHILE ) {
          GetToken( mtokenType, mtokenStr ) ;
          PeekToken( mtokenType, mtokenStr ) ;
          if ( mtokenType == SLP  ) {
            GetToken( mtokenType, mtokenStr ) ;
            if ( Expression() ) {
              PeekToken( mtokenType, mtokenStr ) ;
              if ( mtokenType == SRP ) {
                GetToken( mtokenType, mtokenStr ) ;
                PeekToken( mtokenType, mtokenStr ) ;
                if ( mtokenType == SCOLON ) {
                  return true ;
                } // if
                else {
                  UnexpectedToken( mtokenStr ) ;
                  return false ;
                } // else
              } // if
              else {
                UnexpectedToken( mtokenStr ) ;
                return false ;
              } // else
            } // if
            else return false ;
          } // if
          else {
            UnexpectedToken( mtokenStr ) ;
            return false ;
          } // else	 	
        } // if
        else {
          UnexpectedToken( mtokenStr ) ;
          return false ;
        } // else	 	
      } // if
      else return false ;
    } // else if
    else {
      UnexpectedToken( mtokenStr ) ;
      return false ;
    } // else

    return true ;
  } // Statement()


  bool Expression() {  // expression是由一個或多個basic_expression所組成，其間以','隔開來。
    // : basic_expression { ',' basic_expression }
    if ( Basic_expression() ) {
      PeekToken( mtokenType, mtokenStr ) ;
      while ( mtokenType == COM )  {
        GetToken( mtokenType, mtokenStr ) ;
        if ( Basic_expression() ) 
          PeekToken( mtokenType, mtokenStr ) ;
        else {
          return false ;
        } // else	 	
      } // while
      
      return true ;
    } // if
    else {
      return false ;
    } // else

  } // Expression()

  bool Basic_expression() { 

  //  : Identifier rest_of_Identifier_started_basic_exp
  // | ( PP | MM ) Identifier rest_of_PPMM_Identifier_started_basic_exp
  //  | sign { sign } signed_unary_exp romce_and_romloe
  //  | ( Constant | '(' expression ')' ) romce_and_romloe

    PeekToken( mtokenType, mtokenStr ) ;
    if ( mtokenType == IDENT ) {
      if ( !CheckIDisAssign( mtokenStr ) )
        return false ;
      GetToken( mtokenType, mtokenStr ) ;
      if ( Rest_of_Identifier_started_basic_exp() )
        return true ;
      else
        return false ;
    } // if
    else if ( mtokenType == PP || mtokenType == MM ) {
      GetToken( mtokenType, mtokenStr ) ;
      PeekToken( mtokenType, mtokenStr ) ;
      if ( mtokenType == IDENT ) {
        if ( !CheckIDisAssign( mtokenStr ) )
          return false ;
        GetToken( mtokenType, mtokenStr ) ;
        if ( Rest_of_PPMM_Identifier_started_basic_exp() )
          return true ;
        else
          return false ;
      } // if
      else {
        UnexpectedToken( mtokenStr ) ;
        return false ;
      } // else
    } // else if
    else if ( mtokenType == ADD || mtokenType == SUB || mtokenType == EXCLAIM ) {
      if ( !Sign() ) return false ;
      PeekToken( mtokenType, mtokenStr ) ;
      while ( mtokenType == ADD || mtokenType == SUB || mtokenType == EXCLAIM ) { 
        if ( !Sign() ) return false ;
        PeekToken( mtokenType, mtokenStr ) ;
      } // while 

      if ( Signed_unary_exp() ) {
        if ( Romce_and_romloe() )
          return true ;
        else return false ;
      } // if
      else return false ;
    } // else if
    else if ( mtokenType == CONSTANT ) {
      GetToken( mtokenType, mtokenStr ) ;
      if ( Romce_and_romloe()  )
        return true ;
      else return false ;
    } // else if 
    else if ( mtokenType == SLP ) {
      GetToken( mtokenType, mtokenStr ) ;
      if ( Expression() ) {
        PeekToken( mtokenType, mtokenStr ) ;
        if ( mtokenType == SRP ) {
          GetToken( mtokenType, mtokenStr ) ;
          if ( Romce_and_romloe() )
            return true ;
          else return false ;
        } // if
        else {
          UnexpectedToken( mtokenStr ) ;
          return false ;
        } // else
      } // if
      else return false ;
    } // else if 
    else {
      UnexpectedToken( mtokenStr ) ;
      return false ;
    } // else

  } // Basic_expression()

  bool Rest_of_Identifier_started_basic_exp() {
    //     : [ '[' expression ']' ]  ( assignment_operator basic_expression | [ PP | MM ] romce_and_romloe )
    //  | '(' [ actual_parameter_list ] ')' romce_and_romloe	
    PeekToken( mtokenType, mtokenStr ) ;
    // [ '[' expression ']' ] 
    // cout  <<  mtokenStr << endl ; 
    if ( mtokenType == MLP ) {
      GetToken( mtokenType, mtokenStr ) ;
      if ( Expression() ) {
        PeekToken( mtokenType, mtokenStr ) ;
        if ( mtokenType == MRP ) {
          GetToken( mtokenType, mtokenStr ) ;
          PeekToken( mtokenType, mtokenStr ) ;
        } // if
        else {
          UnexpectedToken( mtokenStr ) ;
          return false ;
        } // else
      } // if
      else
        return false ;
    } // if


    if  ( mtokenType == EQUAL || mtokenType == TE || mtokenType == DE || mtokenType == RE || 
          mtokenType == PE || mtokenType == ME  ) {
      if ( Assignment_operator() ) {
        if ( Basic_expression() )
          return true ;
        else
          return false ;
      } // if
    } // if
    
    else if ( mtokenType == PP || mtokenType == MM || mtokenType == MULT ||
              mtokenType == DIV || mtokenType == REMAIN || mtokenType == ADD ||
              mtokenType == SUB || mtokenType == LS || mtokenType == RS ||
              mtokenType == LEFT || mtokenType == RIGHT || mtokenType == LE || 
              mtokenType == GE || mtokenType == EQ || mtokenType == NEQ ||
              mtokenType == ONEAND || mtokenType == POWER || mtokenType == ONEOR ||
              mtokenType == AND || mtokenType == OR || mtokenType == QUE ) { 

      if ( mtokenType == PP || mtokenType == MM ) {
        GetToken( mtokenType, mtokenStr ) ;
        PeekToken( mtokenType, mtokenStr ) ;
      } // if

      if ( Romce_and_romloe() )
        return true ;
      else return false ;

    } // else if

    else if ( mtokenType == SLP ) {   // 這個是function 
      GetToken( mtokenType, mtokenStr ) ;
      PeekToken( mtokenType, mtokenStr ) ;
      if ( mtokenType == IDENT || mtokenType == PP || mtokenType == MM ||
           mtokenType == ADD || mtokenType == SUB || mtokenType == EXCLAIM || 
           mtokenType == CONSTANT ) {
        if ( Actual_parameter_list() ) {
          PeekToken( mtokenType, mtokenStr ) ;
        } // if 
      } // if 

      if ( mtokenType == SRP ) {
        GetToken( mtokenType, mtokenStr ) ;
        if ( Romce_and_romloe() )
          return true ;
        else return false ;
      } // if
      else {
        UnexpectedToken( mtokenStr ) ;
        return false ;

      } // else
    } // else if

    return true ;  
  } // Rest_of_Identifier_started_basic_exp()

  bool Rest_of_PPMM_Identifier_started_basic_exp() {
    //  : [ '[' expression ']' ] romce_and_romloe 
    PeekToken( mtokenType, mtokenStr ) ;
    if ( mtokenType == MLP ) {
      GetToken( mtokenType, mtokenStr ) ;
      if ( Expression() ) {
        PeekToken( mtokenType, mtokenStr ) ;
        if ( mtokenType == MRP ) {
          GetToken( mtokenType, mtokenStr ) ;
          PeekToken( mtokenType, mtokenStr ) ;
        } // if
        else {
          UnexpectedToken( mtokenStr ) ;
          return false ;
        } // else
      } // if
      else
        return false ;
    } // if

    if ( Romce_and_romloe() )
      return true ;
    else
      return false ;

  } // Rest_of_PPMM_Identifier_started_basic_exp()

  bool Sign() {

    // : '+' | '-' | '!'
    PeekToken( mtokenType, mtokenStr ) ;
    if ( mtokenType == ADD || mtokenType == SUB || mtokenType == EXCLAIM ) {
      GetToken( mtokenType, mtokenStr ) ;
      return true ;
    } // if

    UnexpectedToken( mtokenStr ) ;
    return false ;
  } // Sign()

  bool Actual_parameter_list() {
    //  : basic_expression { ',' basic_expression }
    if ( Basic_expression() ) {
      PeekToken( mtokenType, mtokenStr ) ;
      while ( mtokenType == COM ) {
        GetToken( mtokenType, mtokenStr ) ;
        if ( Basic_expression() ) 
          PeekToken( mtokenType, mtokenStr ) ;
        else {
          return false ;
        } // else	 	
      } // while 

      return true ;
    } // if
    else {
      return false ;
    } // else
  } // Actual_parameter_list()


  bool Assignment_operator() {
    // : '=' | TE | DE | RE | PE | ME
    PeekToken( mtokenType, mtokenStr ) ;

    if  ( mtokenType == EQUAL || mtokenType == TE || mtokenType == DE || mtokenType == RE || 
          mtokenType == PE || mtokenType == ME  ) {
      GetToken( mtokenType, mtokenStr ) ;
      return true ;
    } // if
    else {
      UnexpectedToken( mtokenStr ) ;
      return false ;
    } // else	 	

  } // Assignment_operator()

  bool Romce_and_romloe() { // 即romce_and_romloe
    // : rest_of_maybe_logical_OR_exp [ '?' basic_expression ':' basic_expression ]
    if ( Rest_of_maybe_logical_OR_exp()  ) {
      PeekToken( mtokenType, mtokenStr ) ;
      if ( mtokenType == QUE ) {
        GetToken( mtokenType, mtokenStr ) ;
        if ( Basic_expression() ) {
          PeekToken( mtokenType, mtokenStr ) ;
          if ( mtokenType == COLON ) {
            GetToken( mtokenType, mtokenStr ) ;
            if ( Basic_expression() )
              return true ;
            else return false ;
          } // if
          else {
            UnexpectedToken( mtokenStr ) ;
            return false ;
          } // else	 	
        } // if
        else return false ;
      } // if
      
      return true ;
    } // if
    else
      return false ;


  } // Romce_and_romloe()

  bool Rest_of_maybe_logical_OR_exp() {
      //  : rest_of_maybe_logical_AND_exp { OR maybe_logical_AND_exp }
    if ( Rest_of_maybe_logical_AND_exp() ) {
      PeekToken( mtokenType, mtokenStr ) ;
      while ( mtokenType == OR )  {
        GetToken( mtokenType, mtokenStr ) ;
        if ( Maybe_logical_AND_exp() )
          PeekToken( mtokenType, mtokenStr ) ;
        else return false ;
      } // while

      return true ;
    } // if
    else return false ;

  } // Rest_of_maybe_logical_OR_exp()


  bool Maybe_logical_AND_exp() {
    // : maybe_bit_OR_exp { AND maybe_bit_OR_exp }
    if ( Maybe_bit_OR_exp() ) {
      PeekToken( mtokenType, mtokenStr ) ;
      while ( mtokenType == AND )  {
        GetToken( mtokenType, mtokenStr ) ;
        if ( Maybe_bit_OR_exp() )
          PeekToken( mtokenType, mtokenStr ) ;
        else return false ;
      } // while

      return true ;
    } // if
    else return false ;

  } // Maybe_logical_AND_exp()

  bool Rest_of_maybe_logical_AND_exp() {
    //  : rest_of_maybe_bit_OR_exp { AND maybe_bit_OR_exp }	
    if ( Rest_of_maybe_bit_OR_exp() ) {
      PeekToken( mtokenType, mtokenStr ) ;
      while ( mtokenType == AND )  {
        GetToken( mtokenType, mtokenStr ) ;
        if ( Maybe_bit_OR_exp() )
          PeekToken( mtokenType, mtokenStr ) ;
        else return false ;
      } // while

      return true ;
    } // if
    else return false ;

  } // Rest_of_maybe_logical_AND_exp()


  bool Maybe_bit_OR_exp() {
    //  : maybe_bit_ex_OR_exp { '|' maybe_bit_ex_OR_exp }
    if ( Maybe_bit_ex_OR_exp() ) {
      PeekToken( mtokenType, mtokenStr ) ;
      while ( mtokenType == ONEOR )  {
        GetToken( mtokenType, mtokenStr ) ;
        if ( Maybe_bit_ex_OR_exp() )
          PeekToken( mtokenType, mtokenStr ) ;
        else return false ;
      } // while

      return true ;
    } // if
    else return false ;

  } // Maybe_bit_OR_exp()

  bool Rest_of_maybe_bit_OR_exp() {
    // : rest_of_maybe_bit_ex_OR_exp { '|' maybe_bit_ex_OR_exp }
    if ( Rest_of_maybe_bit_ex_OR_exp() ) {
      PeekToken( mtokenType, mtokenStr ) ;
      while ( mtokenType == ONEOR )  {
        GetToken( mtokenType, mtokenStr ) ;
        if ( Maybe_bit_ex_OR_exp() )
          PeekToken( mtokenType, mtokenStr ) ;
        else return false ;
      } // while

      return true ;
    } // if
    else return false ;

  } // Rest_of_maybe_bit_OR_exp()


  bool Maybe_bit_ex_OR_exp() {
    //  : maybe_bit_AND_exp { '^' maybe_bit_AND_exp } 	
    if ( Maybe_bit_AND_exp() ) {
      PeekToken( mtokenType, mtokenStr ) ;
      while ( mtokenType == POWER )  {
        GetToken( mtokenType, mtokenStr ) ;
        if ( Maybe_bit_AND_exp() )
          PeekToken( mtokenType, mtokenStr ) ;
        else return false ;
      } // while

      return true ;
    } // if
    else return false ;

  } // Maybe_bit_ex_OR_exp()

  bool Rest_of_maybe_bit_ex_OR_exp() {
    // : rest_of_maybe_bit_AND_exp { '^' maybe_bit_AND_exp }
    if ( Rest_of_maybe_bit_AND_exp() ) {
      PeekToken( mtokenType, mtokenStr ) ;
      while ( mtokenType == POWER )  {
        GetToken( mtokenType, mtokenStr ) ;
        if ( Maybe_bit_AND_exp() )
          PeekToken( mtokenType, mtokenStr ) ;
        else return false ;
      } // while

      return true ;
    } // if
    else return false ;

  } // Rest_of_maybe_bit_ex_OR_exp()


  bool Maybe_bit_AND_exp() {
    //  : maybe_equality_exp { '&' maybe_equality_exp }
    if ( Maybe_equality_exp() ) {
      PeekToken( mtokenType, mtokenStr ) ;
      while ( mtokenType == ONEAND )  {
        GetToken( mtokenType, mtokenStr ) ;
        if ( Maybe_equality_exp() )
          PeekToken( mtokenType, mtokenStr ) ;
        else return false ;
      } // while

      return true ;
    } // if
    else return false ;

  } // Maybe_bit_AND_exp()

  bool Rest_of_maybe_bit_AND_exp() {
    // : rest_of_maybe_equality_exp { '&' maybe_equality_exp }
    if ( Rest_of_maybe_equality_exp() ) {
      PeekToken( mtokenType, mtokenStr ) ;
      while ( mtokenType == ONEAND )  {
        GetToken( mtokenType, mtokenStr ) ;
        if ( Maybe_equality_exp() )
          PeekToken( mtokenType, mtokenStr ) ;
        else return false ;
      } // while

      return true ;
    } // if
    else return false ;

  } // Rest_of_maybe_bit_AND_exp()


  bool Maybe_equality_exp() {
    //  : maybe_relational_exp { ( EQ | NEQ ) maybe_relational_exp }
    if ( Maybe_relational_exp() ) {
      PeekToken( mtokenType, mtokenStr ) ;
      while ( mtokenType == EQ || mtokenType == NEQ )  {
        GetToken( mtokenType, mtokenStr ) ;
        if ( Maybe_relational_exp() )
          PeekToken( mtokenType, mtokenStr ) ;
        else return false ;
      } // while

      return true ;
    } // if
    else return false ;

  } // Maybe_equality_exp()

  bool Rest_of_maybe_equality_exp() {
    // : rest_of_maybe_relational_exp { ( EQ | NEQ ) maybe_relational_exp }
    if ( Rest_of_maybe_relational_exp() ) {
      PeekToken( mtokenType, mtokenStr ) ;
      while ( mtokenType == EQ || mtokenType == NEQ )  {
        GetToken( mtokenType, mtokenStr ) ;
        if ( Maybe_relational_exp() )
          PeekToken( mtokenType, mtokenStr ) ;
        else return false ;
      } // while

      return true ;
    } // if
    else return false ;


  } // Rest_of_maybe_equality_exp()


  bool Maybe_relational_exp() {
    //   : maybe_shift_exp 
    //    { ( '<' | '>' | LE | GE ) maybe_shift_exp }
    if ( Maybe_shift_exp() ) {
      PeekToken( mtokenType, mtokenStr ) ;
      while ( mtokenType == LEFT || mtokenType == RIGHT || mtokenType == LE || mtokenType == GE )  {
        GetToken( mtokenType, mtokenStr ) ;
        if ( Maybe_shift_exp() )
          PeekToken( mtokenType, mtokenStr ) ;
        else return false ;
      } // while

      return true ;
    } // if
    else return false ;

  } // Maybe_relational_exp()

  bool Rest_of_maybe_relational_exp() {
    //   : rest_of_maybe_shift_exp 
    //     { ( '<' | '>' | LE | GE ) maybe_shift_exp }
    if ( Rest_of_maybe_shift_exp() ) {
      PeekToken( mtokenType, mtokenStr ) ;
      while ( mtokenType == LEFT || mtokenType == RIGHT || mtokenType == LE || mtokenType == GE )  {
        GetToken( mtokenType, mtokenStr ) ;
        if ( Maybe_shift_exp() )
          PeekToken( mtokenType, mtokenStr ) ;
        else return false ;
      } // while

      return true ;
    } // if
    else return false ;


  } // Rest_of_maybe_relational_exp()


  bool Maybe_shift_exp() {

    //  : maybe_additive_exp { ( LS | RS ) maybe_additive_exp }
    if ( Maybe_additive_exp() ) {
      PeekToken( mtokenType, mtokenStr ) ;
      while ( mtokenType == LS || mtokenType == RS )  {
        GetToken( mtokenType, mtokenStr ) ;
        if ( Maybe_additive_exp() )
          PeekToken( mtokenType, mtokenStr ) ;
        else return false ;
      } // while

      return true ;
    } // if
    else return false ;

  } // Maybe_shift_exp()

  bool Rest_of_maybe_shift_exp() {
    // : rest_of_maybe_additive_exp { ( LS | RS ) maybe_additive_exp }
    if ( Rest_of_maybe_additive_exp() ) {
      PeekToken( mtokenType, mtokenStr ) ;
      while ( mtokenType == LS || mtokenType == RS )  {
        GetToken( mtokenType, mtokenStr ) ;
        if ( Maybe_additive_exp() )
          PeekToken( mtokenType, mtokenStr ) ;
        else return false ;
      } // while

      return true ;
    } // if
    else return false ;

  } // Rest_of_maybe_shift_exp()


  bool Maybe_additive_exp() {
    //  : maybe_mult_exp { ( '+' | '-' ) maybe_mult_exp }
    if ( Maybe_mult_exp() ) {
      PeekToken( mtokenType, mtokenStr ) ;
      while ( mtokenType == ADD || mtokenType == SUB )  {
        GetToken( mtokenType, mtokenStr ) ;
        if ( Maybe_mult_exp() )
          PeekToken( mtokenType, mtokenStr ) ;
        else return false ;
      } // while

      return true ;
    } // if
    else return false ;


  } // Maybe_additive_exp()

  bool Rest_of_maybe_additive_exp() {
    //   : rest_of_maybe_mult_exp { ( '+' | '-' ) maybe_mult_exp }
    if ( Rest_of_maybe_mult_exp() ) {
      PeekToken( mtokenType, mtokenStr ) ;
      while ( mtokenType == ADD || mtokenType == SUB )  {
        GetToken( mtokenType, mtokenStr ) ;
        if ( Maybe_mult_exp() )
          PeekToken( mtokenType, mtokenStr ) ;
        else return false ;
      } // while

      return true ;
    } // if
    else return false ;

  } // Rest_of_maybe_additive_exp()


  bool Maybe_mult_exp() {
    // : unary_exp rest_of_maybe_mult_exp
    if ( Unary_exp() ) {
      if ( Rest_of_maybe_mult_exp() )
        return true ;
      else return false ;
    } // if

    return false ;
  } // Maybe_mult_exp()

  bool Rest_of_maybe_mult_exp() {
    // : { ( '*' | '/' | '%' ) unary_exp }  /* could be empty ! */

    PeekToken( mtokenType, mtokenStr ) ; // 因為不一定要有 '*' | '/' | '%' , 所以不需檢查EOF
    while ( mtokenType == MULT || mtokenType == DIV || mtokenType == REMAIN ) {
      GetToken( mtokenType, mtokenStr ) ;
      if ( Unary_exp() )
        PeekToken( mtokenType, mtokenStr ) ;
      else
        return false ;

    } // while 

    return true ;

  } // Rest_of_maybe_mult_exp()

  bool Unary_exp() {
    //  : sign { sign } signed_unary_exp
    //  | unsigned_unary_exp
    // | ( PP | MM ) Identifier [ '[' expression ']' ]	
    PeekToken( mtokenType, mtokenStr ) ;
    if ( mtokenType == ADD || mtokenType == SUB || mtokenType == EXCLAIM ) {
      if ( !Sign() ) return false ; 
      PeekToken( mtokenType, mtokenStr ) ;

      while ( mtokenType == ADD || mtokenType == SUB || mtokenType == EXCLAIM ) {
        if ( !Sign() ) return false ;
        PeekToken( mtokenType, mtokenStr ) ;
      } // while

      if ( Signed_unary_exp() ) return true ;
      else return false ;
    } // if
    else if ( mtokenType == IDENT || mtokenType == CONSTANT || mtokenType == SLP ) {
      if ( !Unsigned_unary_exp() )
        return false ;
      return true ;
    } // else if


    else if ( mtokenType == PP || mtokenType == MM ) {
      GetToken( mtokenType, mtokenStr ) ;
      PeekToken( mtokenType, mtokenStr ) ;
      if ( mtokenType == IDENT ) {
        if ( !CheckIDisAssign( mtokenStr ) )
          return false ;
        GetToken( mtokenType, mtokenStr ) ;
        PeekToken( mtokenType, mtokenStr ) ;       // 不一定要有下一個
        if ( mtokenType == MLP ) {
          GetToken( mtokenType, mtokenStr ) ;
          if ( Expression() ) // 一定要有 
            PeekToken( mtokenType, mtokenStr ) ; 
          else
            return false ;
          if ( mtokenType == MRP ) {
            GetToken( mtokenType, mtokenStr ) ;
          } // if
          else {
            UnexpectedToken( mtokenStr ) ;
            return false ;
          } // else	
        } // if 
      } // if
      else {
        UnexpectedToken( mtokenStr ) ; 
        return false ;
      } // else	

      return true ;
    } // else if
    else {
      UnexpectedToken( mtokenStr ) ;
      return false ;
    } // else	
  } // Unary_exp()


  bool Signed_unary_exp() { 
    //  : Identifier [ '(' [ actual_parameter_list ] ')' | '[' expression ']'  ]
    // | Constant 
    // | '(' expression ')'
    // Identifier [ '(' [ actual_parameter_list ] ')' | '[' expression ']'  ]
    PeekToken( mtokenType, mtokenStr ) ;
    if ( mtokenType == IDENT ) {
      if ( !CheckIDisAssign( mtokenStr ) )
        return false ; 
      GetToken( mtokenType, mtokenStr ) ;
      PeekToken( mtokenType, mtokenStr ) ;

      if ( mtokenType == SLP ) {  // 可以有也可以不要有  
        GetToken( mtokenType, mtokenStr ) ;
        PeekToken( mtokenType, mtokenStr ) ; 
        if ( Actual_parameter_list() ) {  // 可以有也可以不要有 
          PeekToken( mtokenType, mtokenStr ) ;
        } // if

        if ( mtokenType == SRP ) {  // 一定要有 
          GetToken( mtokenType, mtokenStr ) ;
        } // if
        else {
          UnexpectedToken( mtokenStr ) ;
          return false ;
        } // else	
      } // if

      else if ( mtokenType == MLP ) { // 可以有也可以不要有 
        GetToken( mtokenType, mtokenStr ) ;
        if ( Expression() ) {  // 一定要有 
          PeekToken( mtokenType, mtokenStr ) ;
          if ( mtokenType == MRP ) {  // 一定要有 
            GetToken( mtokenType, mtokenStr ) ;
          } // if
          else {
            UnexpectedToken( mtokenStr ) ;
            return false ;
          } // else	
        } // if 
        else
          return false ;
      } // else if

      return true ;
    } // if

    else if ( mtokenType == CONSTANT ) {
      GetToken( mtokenType, mtokenStr ) ; 
      return true ;
    } // else if

    // '(' expression ')'
    else if ( mtokenType == SLP ) {
      GetToken( mtokenType, mtokenStr ) ; 
      if ( Expression() ) {
        PeekToken( mtokenType, mtokenStr ) ;
        if ( mtokenType == SRP ) {
          GetToken( mtokenType, mtokenStr ) ;
        } // if
        else {
          UnexpectedToken( mtokenStr ) ;
          return false ;
        } // else	
      } // if 

      return true ;
    } // else if
    else {
      UnexpectedToken( mtokenStr ) ;
      return false ;
    } // else	

  } // Signed_unary_exp()


  bool Unsigned_unary_exp() { 
    // : Identifier [ '(' [ actual_parameter_list ] ')' 
    //             |
    //             [ '[' expression ']' ] [ ( PP | MM ) ]
    //           ]
    // | Constant 
    // | '(' expression ')'

    PeekToken( mtokenType, mtokenStr ) ;
    if ( mtokenType == IDENT ) {                 // 一定要有 ID 
      if ( !CheckIDisAssign( mtokenStr ) )
        return false ;
      GetToken( mtokenType, mtokenStr ) ;
      PeekToken( mtokenType, mtokenStr ) ;
      if ( mtokenType == SLP ) {    // 可以有 ( 也可以不用 
        GetToken( mtokenType, mtokenStr ) ;
        PeekToken( mtokenType, mtokenStr ) ;
        if ( Actual_parameter_list() ) {  // 可以有也可以不用  
          PeekToken( mtokenType, mtokenStr ) ;
        } // if

        if ( mtokenType == SRP ) {
          GetToken( mtokenType, mtokenStr ) ;
        } // if
        else {
          UnexpectedToken( mtokenStr ) ;
          return false ;
        } // else	

      } // if
      else if ( mtokenType == MLP || mtokenType == PP || mtokenType == MM ) {
        if ( mtokenType == MLP ) {
          GetToken( mtokenType, mtokenStr ) ;
          if ( Expression() ) {   // 一定要有Expression
            PeekToken( mtokenType, mtokenStr ) ;
            if ( mtokenType == MRP ) {
              GetToken( mtokenType, mtokenStr ) ;
              PeekToken( mtokenType, mtokenStr ) ;
            } // if
            else {
              UnexpectedToken( mtokenStr ) ;
              return false ;
            } // else	
          }  // if
          else
            return false ;
        } // if

        if ( mtokenType == PP || mtokenType == MM ) {
          GetToken( mtokenType, mtokenStr ) ;
        } // if
      } // else if

      return true ;
    } // if

    else if ( mtokenType == CONSTANT ) {
      GetToken( mtokenType, mtokenStr ) ;
      return true ;
    } // else if

    // '(' expression ')'
    else if ( mtokenType == SLP ) {
      GetToken( mtokenType, mtokenStr ) ;
      if ( Expression() ) {
        PeekToken( mtokenType, mtokenStr ) ;
        if ( mtokenType == SRP ) {
          GetToken( mtokenType, mtokenStr ) ;
        } // if
        else {
          UnexpectedToken( mtokenStr ) ;
          return false ;
        } // else
      } // if
      else
        return false ;

      return true ;
    } // else if
    else {
      UnexpectedToken( mtokenStr ) ;
      return false ;
    } // else

  } // Unsigned_unary_exp()


  void UnrecognizedToken( string myGetToken ) {

    int line = mscanner.GetLine() ;
    cout << "Line " << line << " : " ;
    cout <<  "unrecognized token with first char : '" << myGetToken << "'" << endl ;
  } // UnrecognizedToken()

  void UnexpectedToken( string myGetToken ) {
    int line = mscanner.GetLine() ;
    cout << "Line " << line << " : " ;
    cout << "unexpected token : '" << myGetToken << "'" << endl ;
  } // UnexpectedToken()

  void UndefinedToken( string myGetToken ) {
    int line = mscanner.GetLine() ;
    cout << "Line " << line << " : " ;
    cout << "undefined identifier : '" << myGetToken << "'" << endl ;

  } // UndefinedToken()

  void GetToken( TokenType & mtokenType, string & mtokenStr ) {
    int pos = mscanner.myTotalToken.size() ;
    mscanner.GetToken() ;
    myGetToken.assign( mscanner.myTotalToken.begin(), mscanner.myTotalToken.end() ) ;
    if ( !myGetToken.empty() ) {
      mtokenType = myGetToken[pos].tokenType ;
      mtokenStr = myGetToken[pos].tokenStr ;
    } // if

  // cout << getTextForEnum(mtokenType) << " type ---"  << mtokenValue << endl ;
  // mscanner.myTotalToken.clear() ;
  } // GetToken()


  void PeekToken( TokenType & mtokenType, string & mtokenStr ) {

    Token tempToken ;
    tempToken = mscanner.PeekToken() ;
    mtokenType = tempToken.tokenType ;
    mtokenStr = tempToken.tokenStr ;

    if ( tempToken.tokenType == NONE )
      UnrecognizedToken( tempToken.tokenStr ) ;
    // cout << getTextForEnum(mtokenType) << " Ptype " << endl ;
  } // PeekToken()

};

void Parser::Init() {
  // Scannar mscanner ;
  mscanner.Init() ;
  mscanner.myTotalToken.clear() ;
  mTempFunctionList.ident = "" ;
  mTempFunctionList.type = "";
  mTempFunctionList.statements.clear(); 
  mTempFunctionList.reference.clear() ;
  mIs_Declaration = false ;
  mhaveF = false ;
  // TempFunctionList.vList.clear(); 
} // Parser::Init()

 
int main() {
  Scannar scannar ;
  Parser parser ;
  int uTestNum = 0 ;
 
  char ch = '\0';
  parser.Init() ;
 
  scanf( "%d%c", &uTestNum, &ch ) ;
  cout << "Our-C running ...\n" ;
  cout << "> " ;

  while ( parser.User_input() ) {
    cout << "> " ;
    
  } // while 
 
  printf( "Our-C exited ..." ) ;
} // main()
