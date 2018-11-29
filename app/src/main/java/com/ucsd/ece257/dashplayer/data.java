package com.ucsd.ece257.dashplayer;

//simple class to hold data
public class data {
    public int nant;
    public int MCS0; // 1 - 16QAM. 2 - 64QAM. 3 - QPSK
    public int MCS1;
    public int TBS0;
    public int TBS1;
    public int RB11;
    public int RB10;
    public int RB01;
    public int RB00;
    public double RSRQ1;
    public double RSRQ0;

    public data(int _nant, int _MCS0, int _MCS1, int _TBS0, int _TBS1, int _RB11, int _RB10, int _RB01, int _RB00, double _RSRQ1,double _RSRQ0){
        this.nant = _nant;
        this.MCS0 = _MCS0;
        this.MCS1 = _MCS1;
        this.TBS0 = _TBS0;
        this.TBS1 = _TBS1;
        this.RB11 = _RB11;
        this.RB10 = _RB10;
        this.RB01 = _RB01;
        this.RB00 = _RB00;
        this.RSRQ1 = _RSRQ1;
        this.RSRQ0 = _RSRQ0;
    }

}
