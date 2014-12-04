#include "Scene.h"
#include "Ray.h"
#include "Shape.h"
#include <iostream>
#include <fstream>

#include "EasyBMP.h"
#include "EasyBMP_BMP.h"
#include "EasyBMP_DataStructures.h"
#include "EasyBMP_VariousBMPutilities.h"
#include "EasyBMP.cpp"


Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::AddLightSource(LightSource lightSource)
{
	_lightSources.push_back(lightSource);
}

std::vector<LightSource> Scene::LightSources()
{
	return _lightSources;
}

void Scene::AddObserver(Vector observer)
{
	_observer = observer;
}

void Scene::AddViewPort(Viewport viewport)
{
	_viewport = viewport;
}

void Scene::Populate(Shape* shape)
{
	sceneObjects.push_back(shape);
}

void Scene::TraceRays()
{
	BMP image;
	image.SetSize(_viewport.GetNumberOfPixels(1), _viewport.GetNumberOfPixels(2));
	image.SetBitDepth(32);
	
	double z = _viewport.GetPosition().GetThirdComponent();

	for (int x = 0; x < _viewport.GetNumberOfPixels(1); x++)
	{
		for (int y = 0; y < _viewport.GetNumberOfPixels(2); y++)
		{
			double xRay = x / (double)_viewport.GetNumberOfPixels(1) * _viewport.GetDimension(1) - _viewport.GetDimension(1) / 2.0;
			double yRay = y / (double)_viewport.GetNumberOfPixels(2) * _viewport.GetDimension(2) - _viewport.GetDimension(2) / 2.0;
			Vector direction(xRay, yRay, z, true);

			Ray ray(_observer, direction);

			image.SetPixel(x, y, TraceRay(ray));
		}
	}
	image.WriteToFile("spheres.bmp");	
}

RGBColour Scene::TraceRay(Ray ray)
{
	RGBColour illumination(0, 0, 0);

	illumination.Red = 0;
	illumination.Green = 0;
	illumination.Blue = 0;

	for (int i = 0; i < sceneObjects.size(); i++)
	{
		double intersection = sceneObjects[i]->Intersection(ray); 
		if (intersection != -1.0)//There is something to see!
		{
			Ray incidentRay = ray.RayLine(intersection);
			Vector surfaceNormal = sceneObjects[i]->SurfaceNormal(incidentRay);			
			Ray reflectedRay = incidentRay.Reflection(surfaceNormal);
			illumination = sceneObjects[i]->Colour() * reflectedRay.Illumination(LightSources()[0]);
			/*if (illumination.Blue > 0 || illumination.Red > 0 || illumination.Green > 0)
			{	
				std::ofstream file;
				file.open("log.txt", std::ios_base::app);

				file << "Intersection: " << intersection << std::endl;
				file << "ray: (" << ray.Direction().GetFirstComponent() << ", " << ray.Direction().GetSecondComponent() << ", " << ray.Direction().GetThirdComponent() << ")" << std::endl;
				file << "incident ray: (" << incidentRay.Direction().GetFirstComponent() << ", " << incidentRay.Direction().GetSecondComponent() << ", " << incidentRay.Direction().GetThirdComponent() << ")" << std::endl;
				file << "surface normal ray: (" << surfaceNormal.GetFirstComponent() << ", " << surfaceNormal.GetSecondComponent() << ", " << surfaceNormal.GetThirdComponent() << ")" << std::endl;
				file << "reflected ray: (" << reflectedRay.Direction().GetFirstComponent() << ", " << reflectedRay.Direction().GetSecondComponent() << ", " << reflectedRay.Direction().GetThirdComponent() << ")" << std::endl;
				file << "---------------------------------------------------------------------" << std::endl;
			}*/
		}
	}
	return illumination;
}
