//
//  FileReader.hpp
//  NeutronSource
//
//  Created by Annamaria Mazzone on 19/01/22.
//

#ifndef FileReader_hh
#define FileReader_hh

#include <stdio.h>
#include <list>
#include <fstream>
#include "G4Types.hh"
#include "G4String.hh"
#include "G4ThreeVector.hh"
using namespace::std;
class FileReader
{
 public:
   FileReader(G4String fileName);
   ~FileReader();
   
    vector<G4double> GetAnEvent();
private:
   std::ifstream inputFile;
   std::list<vector<G4double>> evList;
};

#endif /* FileReader_hpp */
