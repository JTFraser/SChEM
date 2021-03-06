#include "Process.h"


Process::Process(ParameterPack * pp, double nuCool, double nuDelay, bool printActive)
{
	PP = pp;
	ConstantID = 0;
	WPlusID = 1;
	WMinusID = 2;
	FormationID = 3;
	CoolingID = 4;
	DelayID = 5;
	
	//calculate the SFR parameters
	double mu = PP->stellarDeathParameter.Value;
	double nu = PP->nuSFR.Value;
	double lambda = PP->CoolingFrequency.Value;
	double f = PP->CCSNHotFrac.Value;
	double delta = PP->OutFlowFraction.Value;
	
	double p = mu + nu*(1+delta) + lambda;
	double q = mu*nu*(f+delta) + lambda*(mu + nu);
	
	MBar = PP->galaxyM0.Value;
	std::vector<double> Ci;
	for (int i = 0; i < PP->Betas.size(); ++i)
	{
		double b = PP->Betas[i];
		double numerator = -PP->galaxyMs[i]*(b*b - (mu + lambda)*b + mu*lambda);
		
		double denom= b*b - p * b + q;
		
		
		MBar += PP->galaxyMs[i];
		Ci.push_back(numerator/denom);
		
		
		
	}
	

	//check if in complex or real domain
	double omegaSq = p*p/4 - q;
	ComplexDomain = (omegaSq < 0);
	Omega = sqrt(fabs(omegaSq));
	
	double D = mu*lambda*MBar/q;
	
	double J = PP->galaxyM0.Value - D;
	double K = -nu*PP->galaxyM0.Value*(1.0+delta);
	
	for (int i = 0; i < PP->Betas.size(); ++i)
	{
		J-=Ci[i];
		K+=PP->Betas[i]*(PP->galaxyMs[i] + Ci[i]);
	}
	if (ComplexDomain==false)
	{
		double A = J/2 - (p*J + 2*K)/(4*Omega);
		double B = J/2 + (p*J + 2*K)/(4*Omega);
		

		
		std::vector<double> sfrTemp = {D,A,B,0,0,0};
		for (int i = 0; i < PP->Betas.size(); ++i)
		{
			sfrTemp.push_back(Ci[i]);
		}
		
		SFRVector = ComplexVector(sfrTemp,true);
	}
	else
	{
		std::vector<double> sfrTempReal = {D,J/2,J/2,0,0,0};
		std::vector<double> sfrTempImag = {0,(p*J + 2*K)/(4*Omega),-(p*J + 2*K)/(4*Omega),0,0,0};
		
		for (int i = 0; i < PP->Betas.size(); ++i)
		{
			sfrTempReal.push_back(Ci[i]);
			sfrTempImag.push_back(0);
		}
		
		SFRVector = ComplexVector(sfrTempReal,sfrTempImag);
	}
	
	
	
	SFRVector = PP->nuSFR.Value * SFRVector;
	
	//generate power series
	
	double rTrig = Omega;
	double iTrig = 0;
	if (ComplexDomain)
	{
		iTrig = Omega;
		rTrig = 0;
	}
	double modifiedNu = PP->sfrModifier.Value * 	 PP->nuSFR.Value;

	std::vector<double> realPowers =  {0,p/2 + rTrig, p/2 - rTrig, modifiedNu,nuCool, nuDelay};
	std::vector<double> imaginaryPowers = {0,iTrig, -iTrig,0,0,0};

	
	for (int n = 0; n < Ci.size(); ++n)
	{
		realPowers.push_back(PP->Betas[n]);
		imaginaryPowers.push_back(0.0);
	}
	
	for (int i = 0; i < realPowers.size()-1; ++i)
	{
		for (int j = i+1; j < realPowers.size(); ++j)
		{
			bool samePower = (fabs(realPowers[j] - realPowers[i])<10e-8) && fabs(imaginaryPowers[i] - imaginaryPowers[j]) < 10e-8;
			std::vector<std::string> powNames = {"Constant","Omega plus", "Omega minus", "nuSFR(mod)", "nuCool", "nuDelay","ob1","ob2"};
			if (samePower)
			{
				if (printActive)
				{
					std::cout << i << "  " << j << "  " << realPowers.size() << "   " << powNames.size() << std::endl;
					std::cout << "You have provided two powers of the same value (" << powNames[i] << "=" << realPowers[i] << " & " << powNames[j] << " = " << realPowers[j] << "), this is being corrected." << std::endl;
				}
				realPowers[i] = realPowers[i] * 1.01;
				imaginaryPowers[i] = imaginaryPowers[i] * 0.99;
			}
		}
	}
	
	Powers = ComplexVector(realPowers, imaginaryPowers);
	

}


ComplexVector Process::E(double t)
{
	std::vector<double> realOutput = std::vector(Powers.size(), 0.0);
	std::vector<double> imagOutput = std::vector(Powers.size(), 0.0);

	for (int i = 0; i < Powers.size(); ++i)
	{
		realOutput[i] = exp(- Powers.Real[i] * t) * cos(-Powers.Imaginary[i]*t);
		imagOutput[i] = exp(- Powers.Real[i] * t) * sin(-Powers.Imaginary[i]*t);
	}
	return ComplexVector(realOutput,imagOutput);
}

ComplexVector Process::F(double t)
{
	std::vector<double> realOutput = std::vector(Powers.size(), 0.0);
	std::vector<double> imagOutput = std::vector(Powers.size(), 0.0);
	
	for (int i = 0; i < Powers.size(); ++i)
	{
		if (i > 0)
		{
			realOutput[i] = exp(- Powers.Real[i] * t) * cos(-Powers.Imaginary[i]*t);
			imagOutput[i] = exp(- Powers.Real[i] * t) * sin(-Powers.Imaginary[i]*t);
		}
		else
		{
			realOutput[i] = t;
		}
	}
	return ComplexVector(realOutput,imagOutput);
	
}

ComplexVector Process::Integrator(int freqID)
{
	ComplexVector newVec = ComplexVector(Powers.size());
	
	
	for (int i = 0; i < Powers.size(); ++i)
	{
		if (i !=freqID)
		{
			double ar = Powers.Real[freqID];
			double ai = Powers.Imaginary[freqID];
			double br = Powers.Real[i];
			double bi = Powers.Imaginary[i];
			
			double denom = (ar - br)*(ar - br) + (ai - bi)*(ai - bi);
			
			newVec.Real[i] = (ar - br)/denom;
			newVec.Imaginary[i] = -(ai - bi)/denom;
		}
	}
	return newVec;
}


ComplexVector Process::TotalIntegrator()
{
	ComplexVector newVec = ComplexVector(Powers.size());
	
	for (int i = 0; i < Powers.size(); ++i)
	{
		if (i > 0)
		{
			double a= Powers.Real[i];
			double b = Powers.Imaginary[i];
			
			if (a*a + b*b > 0)
			{
				newVec.Real[i] = -a/(a*a + b*b);
				newVec.Imaginary[i] = b/(a*a + b*b);
			}
		}
		else
		{
			newVec.Real[i] = 1.0;
		}
	}
	
	return newVec;
}

ComplexVector Process::CCSNStyleVector()
{
		
	ComplexVector H = Integrator(CoolingID);
	ComplexVector decayTerm = ComplexVector(SFRVector.size());
	
	decayTerm.Real[CoolingID] = - RealDot(SFRVector,H);
	
	ComplexVector Q  = hotFrac * ( Hadamard(SFRVector, H) + decayTerm);
	ComplexVector M = (1.0 - hotFrac) * SFRVector + Powers.Real[CoolingID] * Q;
	
	ComplexVector H2 = Integrator(FormationID);
	
	
	ComplexVector newVec = Hadamard(M,H2);
	
	newVec.Real[FormationID] -= RealDot(M,H2);
	
	return newVec;
}

ComplexVector Process::GenericIntegrator(ComplexVector input, int id)
{
	ComplexVector H = Integrator(id);
	ComplexVector d1 = Hadamard(input, H );
	
	d1.Real[id] = - RealDot(input, H);
	
	return d1;
}



