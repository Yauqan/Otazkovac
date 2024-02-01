#include <string>
#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <fstream>
#include <vector>
#include <Windows.h>
#include <time.h>
#include <thread>
#include <chrono>
#include <algorithm>

#define podilZakazanychOtazek 2

class otazky
{
  private:
    int N;
    std::vector<int> cislaPoslednichOtazek;
  public:
    std::vector<std::wstring> zneniOtazky;
    
    void nactiOtazky ( std::wifstream & f ) //inicializace
    {
      _setmode(_fileno(stdout), _O_U16TEXT);
      zneniOtazky.clear();
      cislaPoslednichOtazek.clear();
      if ( f.fail() == false )
      {
        std::wstring TMP;
        while ( f.eof() == false )
        {
          std::getline ( f, TMP );
          if ( TMP != L"" )
          zneniOtazky.push_back(TMP);
        }
      }
      else
      {
        std::wcout << L"Nezdarilo se precist soubor!" << std::endl;
      }
      N = zneniOtazky.size();
      std::wcout << L"Pocet otazek = " << N << std::endl;
      int j;
      for ( j = 0; j < N/podilZakazanychOtazek; j-=-1 )
      {
        cislaPoslednichOtazek.push_back(-1);
      }
    }
    void napisNahodnouOtazku () //vypis nahodne otazky, otazky se nesmi prilis opakovat
    {
      while (true)
      {
        int j = rand()%N;
        if ( std::find ( cislaPoslednichOtazek.begin(), cislaPoslednichOtazek.end(), j ) == cislaPoslednichOtazek.end() )
        {
          system("cls");
          std::wcout << L"\n\n\t\t";
          std::wcout << zneniOtazky[j] << std::endl;
          cislaPoslednichOtazek.erase(cislaPoslednichOtazek.begin());
          cislaPoslednichOtazek.push_back(j);
          if ( cislaPoslednichOtazek.size() != N/podilZakazanychOtazek )
          {
            std::wcout << "Error, cislaPoslednichOtazek.size() = " << cislaPoslednichOtazek.size() << std::endl;
          }
          break;
        }
      }
    }
};

int main (void)
{
  std::wifstream f;
  f.open("pool.txt", std::wifstream::in);
  otazky TEST;
  TEST.nactiOtazky ( f );
  f.close();
  
  long int casNaOtazku;
  
  std::ifstream c;
  c.open("conf.settings", std::ifstream::in);
  if ( c.fail() == true )
  {
    std::wcout << "Error: nepodarilo se otevrit soubor conf.settings" << std::endl;
    system("pause");
    return -1;
  }
  c >> casNaOtazku;
  casNaOtazku = casNaOtazku * 1000;
  
  srand(time(0));
  while ( true )
  {
    TEST.napisNahodnouOtazku ();
    std::this_thread::sleep_for ( std::chrono::milliseconds (casNaOtazku) );
  }
}
