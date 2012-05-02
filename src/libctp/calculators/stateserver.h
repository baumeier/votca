/*
 *            Copyright 2009-2012 The VOTCA Development Team
 *                       (http://www.votca.org)
 *
 *      Licensed under the Apache License, Version 2.0 (the "License")
 *
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */


#ifndef __STATESERVER_H
#define __STATESERVER_H


#include <votca/ctp/qmcalculator.h>




namespace votca { namespace ctp {

class StateServer : public QMCalculator
{
public:

    StateServer() { };
   ~StateServer() { };

    string Identify() { return "State Server"; }

    void Initialize(Topology *top, Property *options);
    bool EvaluateFrame(Topology *top);

    void DownloadTopology(FILE *out, Topology *top);
    void DownloadSegments(FILE *out, Topology *top);
    void DownloadPairs(FILE *out, Topology *top);
    void DownloadEList(FILE *out, Topology *top);
    void DownloadIList(FILE *out, Topology *top);
    void DownloadCoords(FILE *out, Topology *top) { };

private:

    string _outfile;
    string _pdbfile;
    vector< string > _keys;

};


void StateServer::Initialize(Topology *top, Property *opt) {
    
    string tag = "options.stateserver";

    // Tabular output
    if ( opt->exists(tag+".out") ) {
        _outfile = opt->get(tag+".out").as< string > ();
    }
    else {
        _outfile = "stateinfo.out";
    }
    // PDB output
    if ( opt->exists(tag+".pdb") ) {
        _pdbfile = opt->get(tag+".pdb").as< string > ();
    }
    else {
        _pdbfile = "system.pdb";
    }

    string keys = "";
    if ( opt->exists(tag+".keys") ) {
        keys = opt->get(tag+".keys").as< string > ();
    }

    Tokenizer tok_keys(keys, " ");
    tok_keys.ToVector(_keys);

}

bool StateServer::EvaluateFrame(Topology *top) {

    // ++++++++++++++++++++++++ //
    // Topology - Sites - Pairs //
    // ++++++++++++++++++++++++ //

    vector< string > ::iterator key;    
    bool writeTrajectory = false;
    
    string outfile = boost::lexical_cast<string>(top->getDatabaseId())+ "_"
                     + _outfile;


    FILE *out = NULL;
    out = fopen(outfile.c_str(), "w");
    for (key = _keys.begin(); key < _keys.end(); key++ ) {

        cout << endl << "... ... Write " << flush;

        if (*key == "topology") {

                cout << "topology, ";

                fprintf(out, "           # +++++++++++++++++++ # \n");
                fprintf(out, "           # MD|QM Topology Info # \n");
                fprintf(out, "           # +++++++++++++++++++ # \n\n");

                DownloadTopology(out, top);
        }
        else if (*key == "sites") {

                cout << endl << "sites, ";

                fprintf(out, "           # ++++++++++++++++++++ # \n");
                fprintf(out, "           # Segments in Database # \n");
                fprintf(out, "           # ++++++++++++++++++++ # \n\n");

                DownloadSegments(out, top);
        }

        else if (*key == "pairs") {

                cout << endl << "pairs, ";

                fprintf(out, "           # +++++++++++++++++ # \n");
                fprintf(out, "           # Pairs in Database # \n");
                fprintf(out, "           # +++++++++++++++++ # \n\n");

                DownloadPairs(out, top);

        }

        else if (*key == "ilist") {

                cout << endl << "integrals, ";

                fprintf(out, "           # +++++++++++++++++++++ # \n");
                fprintf(out, "           # Integrals in Database # \n");
                fprintf(out, "           # +++++++++++++++++++++ # \n\n");

                DownloadIList(out, top);

        }

        else if (*key == "elist") {

                cout << endl << "energies, ";

                fprintf(out, "           # +++++++++++++++++++++++++ # \n");
                fprintf(out, "           # Site energies in Database # \n");
                fprintf(out, "           # +++++++++++++++++++++++++ # \n\n");

                DownloadEList(out, top);

        }

        else {
                cout << "ERROR (Invalid key " << *key << ") ";
        }

        cout << "done. " << flush;

        fprintf(out, "\n\n");
    }
    fclose(out);


    if (writeTrajectory) {

    // ++++++++++++++++++ //
    // MD, QM Coordinates //
    // ++++++++++++++++++ //
        // Segments
        string pdbfile = boost::lexical_cast<string>(top->getDatabaseId())
                         + "_conjg_" + _pdbfile;

        out = fopen(pdbfile.c_str(), "w");
        top->WritePDB(out);
        fclose(out); 


        // Fragments
        pdbfile = boost::lexical_cast<string>(top->getDatabaseId())
                         + "_rigid_" + _pdbfile;

        out = fopen(pdbfile.c_str(), "w");
        vector<Segment*> ::iterator segIt;
        for (segIt = top->Segments().begin();
             segIt < top->Segments().end();
             segIt++) {
            Segment *seg = *segIt;
            seg->WritePDB(out);
        }
        fclose(out);

    }

}

void StateServer::DownloadTopology(FILE *out, Topology *top) {

    fprintf(out, "Topology Database ID %3d \n", top->getDatabaseId());
    fprintf(out, "  Periodic Box: %2.4f %2.4f %2.4f | %2.4f %2.4f %2.4f "
                 "| %2.4f %2.4f %2.4f \n",
            top->getBox().get(0,0),
            top->getBox().get(0,1),
            top->getBox().get(0,2),
            top->getBox().get(1,0),
            top->getBox().get(1,1),
            top->getBox().get(1,2),
            top->getBox().get(2,0),
            top->getBox().get(2,1),
            top->getBox().get(2,2) );

    fprintf(out, "  Step number %7d \n", top->getStep());
    fprintf(out, "  Time          %2.3f \n", top->getTime());
    fprintf(out, "  # Molecules %7d \n", top->Molecules().size());
    fprintf(out, "  # Segments  %7d \n", top->Segments().size());
    fprintf(out, "  # Atoms     %7d \n", top->Atoms().size());
    fprintf(out, "  # Pairs     %7d \n", top->NBList().size());
    
}

void StateServer::DownloadSegments(FILE *out, Topology *top) {

    vector< Segment* > ::iterator segit;

    for (segit = top->Segments().begin();
            segit < top->Segments().end();
            segit++) {

        Segment *seg = *segit;
        fprintf(out, "SiteID %5d %5s | xyz %8.3f %8.3f %8.3f "
                     "| SiteE(intra) C %2.4f A %2.4f "
                     "| Lambdas: NC %2.4f CN %2.4f NA %2.4f AN %2.4f "
                     "| SiteE(pol+estat) C %2.4f N %2.4f A %2.4f     \n",
                seg->getId(),
                seg->getName().c_str(),
                seg->getPos().getX(),
                seg->getPos().getY(),
                seg->getPos().getZ(),
                seg->getU_cC_nN(1),
                seg->getU_cC_nN(-1),
                seg->getU_nC_nN(1),
                seg->getU_cN_cC(1),
                seg->getU_nC_nN(-1),
                seg->getU_cN_cC(-1),
                seg->getEMpoles(1),
                seg->getEMpoles(0),
                seg->getEMpoles(-1) );
    }
}

void StateServer::DownloadPairs(FILE *out, Topology *top) {
    QMNBList::iterator nit;

    for (nit = top->NBList().begin();
         nit != top->NBList().end();
         nit++) {

        QMPair *pair = *nit;

        int ghost;
        if (pair->HasGhost()) { ghost = 1; }
        else { ghost = 0; }

        fprintf(out, "PairID %5d  | Seg1 %4d Seg2 %4d dR %2.4f PBC? %1d | "
                     "lOuter %1.4f J2 %1.8f r12 %2.4f r21 %2.4f \n",
                pair->getId(),
                pair->first->getId(),
                pair->second->getId(),
                pair->Dist(),
                ghost,
                0.0, // pair->getLambdaO(),
                0.0, // pair->calcJeff2(),
                0.0, // pair->getRate12(),
                0.0 ); // pair->getRate21() );
    }
}

void StateServer::DownloadIList(FILE *out, Topology *top) {
    QMNBList::iterator nit;
    for (nit = top->NBList().begin();
         nit != top->NBList().end();
         ++nit) {
        QMPair *pair = *nit;

        fprintf(out, "%5d %5d %5d e %4.7e h %4.7e \n",
        pair->getId(), 
        pair->Seg1()->getId(),
        pair->Seg2()->getId(),
        pair->getJeff2(-1),
        pair->getJeff2(+1));
    }
}

void StateServer::DownloadEList(FILE *out, Topology *top) {
    ;
}

}}





#endif
