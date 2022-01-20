//
//  FileReader.cpp
//  NeutronSource
//
//  Created by Annamaria Mazzone on 19/01/22.
//
#include "G4Types.hh"
#include "FileReader.hh"
#include "G4String.hh"
#include "G4ThreeVector.hh"
#include <vector>

FileReader::FileReader(G4String fileName)
{
    inputFile.open(fileName.data()); }

FileReader::~FileReader()
{ inputFile.close(); }

vector<G4double> FileReader::GetAnEvent()
{
  if( evList.size() == 0 )
  {
      int ind2=-1;
      double em_v,ep_v,thetam_v,thetap_v,phim_v,phip_v;
      
          while (inputFile >> em_v >> ep_v >> thetam_v >> thetap_v >> phim_v >> phip_v)
          {
             ind2++;
             if(ind2 %5==0){// prende 1/5 delle righe del file
                 vector<G4double> vect{em_v,thetam_v,phim_v,ep_v,thetap_v,phip_v};
                 evList.push_back(vect);
              }
             else continue;
          }
  }
  vector <G4double> ev = evList.front();
  evList.pop_front();
  return ev;
}
