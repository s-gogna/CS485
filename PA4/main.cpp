#include <iostream>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace std;
using namespace cv;

#define PRINT_CSV 0

// Function Declaration
vector<Point2f> readImagePoints(const char[]);

vector<Point3f> readWorldPoints(const char[]);

// Main Program
int main()
{
	// Initialize variables
	char buf[128];
	double calibrateCameraErr;
	vector< vector<Point2f> > imgCoords;
	vector< vector<Point3f> > objCoords;
	Mat cameraMatrix, distCoeffs;
	Mat pointToMat(3, 1, CV_64FC1);
	vector<Mat> rvecs, tvecs;

	// Initialize both vector of vectors
	for( int i = 1; i <= 15; ++i )
	{
		// Read the pixel file
		sprintf( buf, "data/pixel%02d.txt", i );
		imgCoords.push_back( readImagePoints(buf) );

		// Read the world file
		sprintf( buf, "data/world%02d.txt", i );
		objCoords.push_back( readWorldPoints(buf) );
	}

	// Calibrate
	calibrateCameraErr = calibrateCamera( objCoords, imgCoords, Size(640, 480),
		cameraMatrix, distCoeffs,
		rvecs, tvecs );

	// Loop throught the images
	double multiImageSum = 0.0;
	for( int i = 0; i < 15; ++i )
	{
		// Project points
		vector<Point2f> output;
		projectPoints( objCoords[i], rvecs[i], tvecs[i], cameraMatrix, distCoeffs, output );

		// Loop through the output
		double singleImageSum = 0.0;
		for( int j = 0; j < 96; ++j )
		{
			double err = sqrt( 
				pow(output[j].x - imgCoords[i][j].x, 2) + 
				pow(output[j].y - imgCoords[i][j].y, 2) );
			singleImageSum += err;

			#if PRINT_CSV == 1
			cout << i << ',' << err << endl;
			#endif
		}

		// Add to the multi-image error sum
		multiImageSum += singleImageSum / 96.0;

		#if PRINT_CSV == 0
		cout << "Image " << i << ": " << singleImageSum / 96.0 << endl;
		#endif
	}

	#if PRINT_CSV == 0
	cout << "For all images: " << multiImageSum / 15.0 << endl;
	#endif

	// Return
	return 0;
}

// Function implementations
vector<Point2f> readImagePoints(const char filename[])
{
	// Initialize variables
	Point2f temp(0, 0);
	vector<Point2f> result;

	// Open the file
	fstream fin(filename);

	// Loop
	for( int i = 0; i < 96; ++i )
	{
		// Read and push back the point
		fin >> temp.x >> temp.y;
		result.push_back(temp);
	}

	// Return
	return result;
}

vector<Point3f> readWorldPoints(const char filename[])
{
	// Initialize variables
	Point3f temp(0, 0, 0);
	vector<Point3f> result;

	// Open the file
	fstream fin(filename);

	// Loop
	for( int i = 0; i < 96; ++i )
	{
		// Read and push back the point
		fin >> temp.x >> temp.y;
		result.push_back(temp);
	}

	// Return
	return result;
}





