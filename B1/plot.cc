// Include necessary ROOT libraries
#include <iostream>
#include <fstream>
#include <TH1F.h>
#include <TCanvas.h>

void plot(const char* filename = "optical_photons_per_event.text") {
    // Open the text file for reading
    std::ifstream infile(filename);
    
    if (!infile.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return;
    }

    // Create a histogram for the MCA spectrum
    // Adjust the number of bins and range as per your requirements
    TH1F* hMCA = new TH1F("hMCA", "MCA Spectrum; Optical Photons; Counts", 500, 0, 1500);

    // Read the file line by line and fill the histogram
    double photons;
    double calibration_constant = 17.2; // Read the paper of NaI paper folder(NaI paper) 
    double constant_eV_to_keV = 1000.00;

    while (infile >> photons) {
        hMCA->Fill(photons*calibration_constant/constant_eV_to_keV);  // Fill the histogram with the number of photons per event
    }

    // Close the file after reading
    infile.close();

    // Draw the histogram on a canvas
    TCanvas* c1 = new TCanvas("c1", "MCA Spectrum", 800, 600);

    hMCA->Draw();
    // Set axis labels
    hMCA->GetXaxis()->SetTitle("Energy (keV)");
    hMCA->GetYaxis()->SetTitle("Counts");

    // Center axis titles
    hMCA->GetXaxis()->CenterTitle();
    hMCA->GetYaxis()->CenterTitle();

    // Save the histogram as an image (optional)
    //c1->SaveAs("MCA_spectrum.png");
    
    // Optionally, write the histogram to a ROOT file for future analysis
    //TFile* outfile = new TFile("MCA_spectrum.root", "RECREATE");
    //hMCA->Write();
    //outfile->Close();
    
   // std::cout << "MCA Spectrum generated and saved!" << std::endl;

   // Draw the canvas
   c1->Update();
   c1->Draw();
}

