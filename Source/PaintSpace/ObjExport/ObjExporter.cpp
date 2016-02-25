// Fill out your copyright notice in the Description page of Project Settings.

#include "PaintSpace.h"
#include "ObjExporter.h"
#include "StaticMeshResources.h"
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>


ObjExporter::ObjExporter()
{
	DEBUG_DIRECTORY = FString("C:/workSpace/Thesis/PaintSpace/ObjOut"); // should be configurable setting
}

ObjExporter::~ObjExporter()
{
}

void ObjExporter::RegisterStaticMeshComponent(UInstancedStaticMeshComponent* InComponent)
{
	InstancedStaticMeshComponent = InComponent;
}

bool ObjExporter::ExportObjFile()
{
	if (!InstancedStaticMeshComponent)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString("Error: No InstancedStaticMeshComponent Specified"));
		return false;
	}

	FString ObjString = FString("");
	ObjString += "g test\n";
	
	for (FInstancedStaticMeshInstanceData mesh : InstancedStaticMeshComponent->PerInstanceSMData)
	{
		FTransform transform = FTransform(mesh.Transform);
		FVector translation = transform.GetTranslation();

		FPositionVertexBuffer* VertexBuffer = &InstancedStaticMeshComponent->StaticMesh->RenderData->LODResources[0].PositionVertexBuffer;
		if (VertexBuffer)
		{
			const int32 VertexCount = VertexBuffer->GetNumVertices();
			for (int32 Index = 0; Index < VertexCount; Index++)
			{
				const FVector WorldVertexLocation = translation + VertexBuffer->VertexPosition(Index);
				std::stringstream fmt;
				fmt << "v " << std::to_string(WorldVertexLocation.X) << " " << std::to_string(WorldVertexLocation.Y) << " " << std::to_string(WorldVertexLocation.Z) << "\n"; // a bit ugly...
				ObjString += fmt.str().c_str();
			}
		}

	}
	
	FString name = FString("out");

	std::ofstream outfile ("C:/workSpace/Thesis/PaintSpace/ObjOut.obj");
	if (outfile.is_open())
	{
		outfile << TCHAR_TO_ANSI(*ObjString);
		outfile.close();
		return true;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString("()()()"));
		return false;
	}
	// below didn't work, resorted to std lib filestreams, needs cleanup

	/*IPlatformFile& Platformfile = FPlatformFileManager::Get().GetPlatformFile();
	IFileHandle* FileHandle = Platformfile.OpenWrite(*DEBUG_DIRECTORY,true,true);

	if (FileHandle)
	{
		if (FileHandle->Write((const uint8*)TCHAR_TO_ANSI(*ObjString), ObjString.Len()))
		{
			delete FileHandle;
			return true;
		}
	}*/
	//return FFileHelper::SaveStringToFile(ObjString, *DEBUG_DIRECTORY);

	//bool res = SaveStringToFileHelper(DEBUG_DIRECTORY, name, FString(""), true);
	//return res;
}

bool ObjExporter::SaveStringToFileHelper(FString Dir, FString FileName, FString SaveString, bool OverWrite)
{
	// check for dir and create if doesn't already exist
	if (!FPlatformFileManager::Get().GetPlatformFile().DirectoryExists(*Dir))
	{
		FPlatformFileManager::Get().GetPlatformFile().CreateDirectory(*Dir);
		if (!FPlatformFileManager::Get().GetPlatformFile().DirectoryExists(*Dir))
			return false;
	}

	Dir += "/";
	Dir += FileName;

	if (!OverWrite)
	{
		if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*Dir))
		{
			return false;
		}
	}

	return FFileHelper::SaveStringToFile(SaveString, *Dir);
}