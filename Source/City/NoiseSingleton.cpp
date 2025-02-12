// Fill out your copyright notice in the Description page of Project Settings.

#include "City.h"
#include "NoiseSingleton.h"
#include "simplexnoise.h"

NoiseSingleton* NoiseSingleton::instance;

#define MAX(a, b) ( a > b ? a : b)
#define MIN(a, b) ( a < b ? a : b)


NoiseSingleton::NoiseSingleton()
{
	xOffset = FMath::FRandRange(-100000, 100000);
	yOffset = FMath::FRandRange(-100000, 100000);

}

NoiseSingleton::~NoiseSingleton()
{
}

float NoiseSingleton::noise(float x, float y){
	if (!useTexture) {

		float val = SimplexNoise::simplexnoise(noiseScale * x + xOffset, noiseScale*y + yOffset);
		return val * 0.5 + 0.5;

	}
	else {
		FTexture2DMipMap* MyMipMap = &image->PlatformData->Mips[0];
		FByteBulkData* RawImageData = &MyMipMap->BulkData;
		FColor* FormatedImageData = static_cast<FColor*>(RawImageData->Lock(LOCK_READ_ONLY));
		uint32 TextureWidth = MyMipMap->SizeX, TextureHeight = MyMipMap->SizeY;
		FColor PixelColor;

		uint32 intX = (FMath::FloorToInt(x * noiseScale * noiseTextureScale));
		uint32 intY = (FMath::FloorToInt(y * noiseScale * noiseTextureScale));

		if (intX >= TextureWidth || intY >= TextureHeight) {
			RawImageData->Unlock();
			return 0.0f;
		}

		PixelColor = FormatedImageData[intY * TextureWidth + intX];
		RawImageData->Unlock();

		return (PixelColor.R / 255.0f);

	}
}

FVector NoiseSingleton::getStartSuggestion() {
	if (!useTexture)
		return FVector(0, 0, 0);
	else {
		FTexture2DMipMap* MyMipMap = &image->PlatformData->Mips[0];
		FByteBulkData* RawImageData = &MyMipMap->BulkData;
		FColor* FormatedImageData = static_cast<FColor*>(RawImageData->Lock(LOCK_READ_ONLY));
		uint32 TextureWidth = MyMipMap->SizeX, TextureHeight = MyMipMap->SizeY;
		RawImageData->Unlock();

		UE_LOG(LogTemp, Warning, TEXT("start x,y: %f, %f"), (TextureWidth / (noiseTextureScale * noiseScale)), (TextureHeight / (noiseTextureScale * noiseScale)));
		return FVector((TextureWidth / (noiseTextureScale * noiseScale)) / 2, (TextureHeight / (noiseTextureScale * noiseScale)) / 2, 0);
	}
}