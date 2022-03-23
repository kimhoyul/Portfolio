// // Fill out your copyright notice in the Description page of Project Settings.
//
// #pragma once
// #include "StructData.h"
// #include "Utile.generated.h"
//
// void ForEachLoopWithBreak(TArray<FItemTypeAndID> Array, FItemTypeAndID& ArrayElement, int32& Index);
// void Loopbody(int32 LoopCount, int32 LoopIndex, TArray<FItemTypeAndID> Array, FItemTypeAndID& ArrayElement, int32& Index);
//
// ///////////////////////////////////////////////////////////////
// // FItemTypeAndID For Each Loop with Break 함수
// void ForEachLoopWithBreak(TArray<FItemTypeAndID> Array, FItemTypeAndID& ArrayElement, int32& Index)
// {
// 	//로컬 Boolean 임시 변수
// 	bool bBreak;
//
// 	//로컬 Boolean 임시 변수
// 	int32 LoopCounter, ArrayIndex;
//
// 	//임시 변수 초기화
// 	bBreak = false;
// 	LoopCounter = 0;
// 	ArrayIndex = 0;
//
// 	if (Array.Num() < LoopCounter && !bBreak)
// 	{
// 		ArrayElement = Array[ArrayIndex];
// 		Index = ArrayIndex;
// 	}
// 	Loopbody(LoopCounter, ArrayIndex, Array, ArrayElement, Index);
// }
//
// void Loopbody(int32 LoopCount, int32 LoopIndex, TArray<FItemTypeAndID> Array, FItemTypeAndID& ArrayElement, int32& Index);
// {
// 	int32 LoopbodyCounter = LoopCount;
// 	int32 LoopbodyIndex = LoopIndex;
// 	
// 	LoopbodyIndex = LoopbodyCounter;
// 		
// 	LoopbodyCounter += 1;
// 	
// }