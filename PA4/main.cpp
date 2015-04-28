#include <iostream>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace std;
using namespace cv;

// Function Declaration
vector<Point2f> readImagePoints(const char[]);

vector<Point3f> readWorldPoints(const char[]);

Mat convertPoint3fToMat(const Point3f&);

Mat buildExtrinsicParams( const Mat&, const Mat& );

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

	// Loop through the 15 images
//	for( int i = 0; i < 15; ++i )
	for( int i = 0; i < 1; ++i )
	{
		// Calculate the rotation matrix transposed
		Mat rotMat;
		Rodrigues( rvecs[i], rotMat );

		// Calculate the extrinsic parameters
		Mat extrinsicParams = buildExtrinsicParams( rotMat, tvecs[i] );
		Mat projMatrix = cameraMatrix * extrinsicParams;

		// Loop through the 96 points
//		for( int j = 0; j < 96; ++j )
		for( int j = 0; j < 2; ++j )
		{
			// Convert the Point3f world coordinate to a Mat with homogenous coordinates
			Mat pointDiff = convertPoint3fToMat( objCoords[i][j] );
cout << pointDiff << endl;
			// Multiply with the rotMatTrans
			Mat imgVal = projMatrix * pointDiff;
			//cout << imgVal/imgVal.at<double>(2) << endl;
			cout << imgVal << endl;

			// Print out the actual coordinate values
			cout << imgCoords[i][j] << endl;
		}
	}

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

Mat convertPoint3fToMat(const Point3f& point)
{
	// Create a matrix
	Mat res(4, 1, CV_64FC1);

	// Copy the values
	res.at<double>(0) = point.x;
	res.at<double>(1) = point.y;
	res.at<double>(2) = point.z;
	res.at<double>(3) = 1;

	// Return
	return res;
}

Mat buildExtrinsicParams( const Mat& rotMat, const Mat& tvec )
{
	// Initialize variables
	Mat res( 3, 4, CV_64FC1 );

	// Copy the rotation matrix
	rotMat.col(0).copyTo( res.col(0) );
	rotMat.col(1).copyTo( res.col(1) );
	rotMat.col(2).copyTo( res.col(2) );

	// Multiply rotMat with tvec and append its negative
	Mat temp = -( rotMat.t() * tvec );
	temp.copyTo( res.col(3) );

	// Return
	return res;
}





