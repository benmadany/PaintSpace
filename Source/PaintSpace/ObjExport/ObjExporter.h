// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * 
 */
class PAINTSPACE_API ObjExporter
{
public:
	ObjExporter();
	~ObjExporter();
	bool ExportObjFile();
	void RegisterStaticMeshComponent(UInstancedStaticMeshComponent* InComponent);
	bool SaveStringToFileHelper(FString Dir, FString FileName, FString SaveString, bool OverWrite);


private:

	FString DEBUG_DIRECTORY;

	UInstancedStaticMeshComponent* InstancedStaticMeshComponent;
};
