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

#define podilZakazanychOtazek 1

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
        #if podilZakazanychOtazek == 1
        else if ( std::find ( cislaPoslednichOtazek.begin(), cislaPoslednichOtazek.end(), -1 ) == cislaPoslednichOtazek.end() )
        {
          std::wcout << "\n\t Dosly otazky, zacinam znovu:\n";
          cislaPoslednichOtazek.clear();
          int k;
          for ( k = 0; k < N/podilZakazanychOtazek; k-=-1 )
          {
            cislaPoslednichOtazek.push_back(-1);
          }
          std::this_thread::sleep_for ( std::chrono::milliseconds (5000) );
        }
        #endif
      }
    }
};

int main (void)
{
  std::wifstream f;
  std::wstring address;
  
  long int casNaOtazku;
  
  std::wifstream c;
  c.open("conf.settings", std::wifstream::in);
  if ( c.fail() == true )
  {
    std::wcout << "Error: nepodarilo se otevrit soubor conf.settings" << std::endl;
    system("pause");
    return -1;
  }
  c >> casNaOtazku;
  c >> address;
  casNaOtazku = casNaOtazku * 1000;
  f.open(address.data(), std::wifstream::in);
  if ( f.fail() == true )
  {
    std::wcout << "Error: nepodarilo se otevrit soubor na lokalni adrese " << address << '\n';
    system("pause");
    return -1;
  }
  otazky TEST;
  TEST.nactiOtazky ( f );
  f.close();
  
  srand(time(0));
  while ( true )
  {
    TEST.napisNahodnouOtazku ();
    std::this_thread::sleep_for ( std::chrono::milliseconds (casNaOtazku) );
  }
}
