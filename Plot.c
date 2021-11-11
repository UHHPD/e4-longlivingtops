#include "TH1F.h"
#include "TH2F.h"
#include <iostream>
#include "TCanvas.h"
#include "TLegend.h"
#include "TFile.h"
#include "TPad.h"
#include "TPaveLabel.h"
#include "TPaveText.h"
#include "TGraph.h"


#include <fstream>

#include <vector>


double background(double x,double alpha=0.005,double beta=-0.00001, double gamma=0.08, double delta=0.015)
{
	return alpha+beta*x+gamma*exp(-delta*x);
}
void Plot()
{
	TCanvas* c = new TCanvas("c","c",600,600);
	// c->Divide(3,3);
	c->cd();
	c->SetGrid();

	TPad* pad= new TPad("pad","pad",0.01,0.00,0.99,0.99);
	pad->SetLeftMargin(0.12);
	pad->SetRightMargin(0.01);
	pad->SetGrid();
	pad->Draw();
	pad->cd();

	auto* mg= new TMultiGraph();
	auto* gr = new TGraphErrors("MesA.txt", "%lg\t%lg\t%lg");
	gr->SetLineWidth(2);
	gr->SetLineColor(600-3);
	gr->SetMarkerColor(600-3);
	gr->SetMarkerStyle(8);


	auto* gr2 = new TGraph();
  for (double i=0; i<300; i+=0.01) gr2->AddPoint(i, background(i));
	gr2->SetLineWidth(2);
	gr2->SetLineColor(632-3);

	mg->Add(gr,"AP");
	mg->Add(gr2,"AC");

	mg->GetHistogram()->GetYaxis()->SetRangeUser(0, 0.08);
	mg->GetHistogram()->GetXaxis()->SetRangeUser(22, 300);
	mg->GetHistogram()->GetXaxis()->SetTitle("mass [GeV] ");
	mg->GetHistogram()->GetYaxis()->SetTitle("#sigma [pb]");
	mg->GetHistogram()->SetTitle("Exp A");

  mg->Draw("A");
	// gr2->Draw("AC same");
	c->Print("PlotA.pdf");
}
