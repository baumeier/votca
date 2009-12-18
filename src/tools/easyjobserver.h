/* 
 * File:   tofobserver.h
 * Author: ruehle
 *
 * Created on December 9, 2009, 9:39 AM
 */

#ifndef _EASYJOBSERVER_H
#define	_EASYJTOFOBSERVER_H

#include <votca/csg/cgobserver.h>
#include <moo/jcalc.h>
#include "qmtopology.h"

class EasyJObserver
    : public CGObserver
{
public:
    EasyJObserver();
    ~EasyJObserver();

    
    void setQMTopology(QMTopology &qmtop) {_qmtop = &qmtop; }

    /// begin coarse graining a trajectory
    void BeginCG(Topology *top, Topology *top_atom);

    /// end coarse graining a trajectory
    void EndCG();

    /// evaluate current conformation
    void EvalConfiguration(Topology *top, Topology *top_atom = 0);

    void setCutoff(const double & cutoff){
        _cutoff = cutoff;
    }
    void setNNnames(string  nnnames);
protected:
    QMTopology *_qmtop;
    double _cutoff;
    vector <string> _nnnames;
    vector <double> _Js;

    bool MatchNNnames(CrgUnit * crg1, CrgUnit * crg2);
};


#endif	/* _TOFOBSERVER_H */

