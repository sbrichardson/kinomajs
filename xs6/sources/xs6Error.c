/*
 *     Copyright (C) 2010-2015 Marvell International Ltd.
 *     Copyright (C) 2002-2010 Kinoma, Inc.
 *
 *     Licensed under the Apache License, Version 2.0 (the "License");
 *     you may not use this file except in compliance with the License.
 *     You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *     Unless required by applicable law or agreed to in writing, software
 *     distributed under the License is distributed on an "AS IS" BASIS,
 *     WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *     See the License for the specific language governing permissions and
 *     limitations under the License.
 */
#include "xs6All.h"

static void fx_Error(txMachine* the);
static void fx_Error_aux(txMachine* the);
static void fx_Error_toString(txMachine* the);
static void fx_EvalError(txMachine* the);
static void fx_RangeError(txMachine* the);
static void fx_ReferenceError(txMachine* the);
static void fx_SyntaxError(txMachine* the);
static void fx_TypeError(txMachine* the);
static void fx_URIError(txMachine* the);

void fxBuildError(txMachine* the)
{
	txSlot* slot;
	mxPush(mxObjectPrototype);
	slot = fxLastProperty(the, fxNewObjectInstance(the));
	slot = fxNextHostFunctionProperty(the, slot, fx_Error_toString, 0, mxID(_toLocaleString), XS_DONT_ENUM_FLAG);
	slot = fxNextHostFunctionProperty(the, slot, fx_Error_toString, 0, mxID(_toString), XS_DONT_ENUM_FLAG);
	slot = fxNextStringProperty(the, slot, "Error", mxID(_name), XS_DONT_ENUM_FLAG);
	slot = fxNextStringProperty(the, slot, "", mxID(_message), XS_DONT_ENUM_FLAG);
	mxErrorPrototype = *the->stack;
	fxNewHostConstructorGlobal(the, fx_Error, 0, mxID(_Error), XS_GET_ONLY);
	the->stack++;
	mxPush(mxErrorPrototype);
	slot = fxLastProperty(the, fxNewObjectInstance(the));
	slot = fxNextStringProperty(the, slot, "EvalError", mxID(_name), XS_DONT_ENUM_FLAG);
	mxEvalErrorPrototype = *the->stack;
	fxNewHostConstructorGlobal(the, fx_EvalError, 1, mxID(_EvalError), XS_GET_ONLY);
	the->stack++;
	mxPush(mxErrorPrototype);
	slot = fxLastProperty(the, fxNewObjectInstance(the));
	slot = fxNextStringProperty(the, slot, "RangeError", mxID(_name), XS_DONT_ENUM_FLAG);
	mxRangeErrorPrototype = *the->stack;
	fxNewHostConstructorGlobal(the, fx_RangeError, 1, mxID(_RangeError), XS_GET_ONLY);
	the->stack++;
	mxPush(mxErrorPrototype);
	slot = fxLastProperty(the, fxNewObjectInstance(the));
	slot = fxNextStringProperty(the, slot, "ReferenceError", mxID(_name), XS_DONT_ENUM_FLAG);
	mxReferenceErrorPrototype = *the->stack;
	fxNewHostConstructorGlobal(the, fx_ReferenceError, 1, mxID(_ReferenceError), XS_GET_ONLY);
	the->stack++;
	mxPush(mxErrorPrototype);
	slot = fxLastProperty(the, fxNewObjectInstance(the));
	slot = fxNextStringProperty(the, slot, "SyntaxError", mxID(_name), XS_DONT_ENUM_FLAG);
	mxSyntaxErrorPrototype = *the->stack;
	fxNewHostConstructorGlobal(the, fx_SyntaxError, 1, mxID(_SyntaxError), XS_GET_ONLY);
	the->stack++;
	mxPush(mxErrorPrototype);
	slot = fxLastProperty(the, fxNewObjectInstance(the));
	slot = fxNextStringProperty(the, slot, "TypeError", mxID(_name), XS_DONT_ENUM_FLAG);
	mxTypeErrorPrototype = *the->stack;
	fxNewHostConstructorGlobal(the, fx_TypeError, 1, mxID(_TypeError), XS_GET_ONLY);
	the->stack++;
	mxPush(mxErrorPrototype);
	slot = fxLastProperty(the, fxNewObjectInstance(the));
	slot = fxNextStringProperty(the, slot, "URIError", mxID(_name), XS_DONT_ENUM_FLAG);
	mxURIErrorPrototype = *the->stack;
	fxNewHostConstructorGlobal(the, fx_URIError, 1, mxID(_URIError), XS_GET_ONLY);
	the->stack++;
}

void fx_Error(txMachine* the)
{
	if (mxTarget->kind == XS_UNDEFINED_KIND) {
		mxPush(mxErrorPrototype);
		fxNewObjectInstance(the);
		mxPullSlot(mxResult);
	}
	fx_Error_aux(the);
}

void fx_Error_aux(txMachine* the)
{
	txSlot* aProperty;

	if ((mxArgc > 0) && (mxArgv(0)->kind != XS_UNDEFINED_KIND)) {
		aProperty = fxSetProperty(the, fxGetInstance(the, mxResult), mxID(_message), C_NULL);
		aProperty->value.string = fxToString(the, mxArgv(0));
		aProperty->kind = mxArgv(0)->kind;
	}
}

void fx_Error_toString(txMachine* the)
{
	txSlot* anError;
	txSlot* aProperty;
	txInteger aLength;
	
	anError = fxGetInstance(the, mxThis);
	aProperty = fxGetProperty(the, anError, mxID(_name));
	mxPushSlot(aProperty);
	if (the->stack->kind == XS_UNDEFINED_KIND) 
		fxCopyStringC(the, the->stack, "Error");
	else	
		fxToString(the, the->stack);
	aProperty = fxGetProperty(the, anError, mxID(_message));
	mxPushSlot(aProperty);
	if (the->stack->kind == XS_UNDEFINED_KIND) 
		fxCopyStringC(the, the->stack, "");
	else	
		fxToString(the, the->stack);
	aLength = c_strlen(the->stack->value.string);
	if (aLength) {
		aLength += c_strlen((the->stack + 1)->value.string) + 2;
		mxResult->value.string = (txString)fxNewChunk(the, aLength + 1);
		mxResult->kind = XS_STRING_KIND;
		c_strcpy(mxResult->value.string, (the->stack + 1)->value.string);
		c_strcat(mxResult->value.string, ": ");
		c_strcat(mxResult->value.string, the->stack->value.string);
		the->stack++;
		the->stack++;
	}
	else {
		the->stack++;
		mxPullSlot(mxResult);
	}
}

void fx_EvalError(txMachine* the)
{
	if (mxResult->kind == XS_UNDEFINED_KIND) {
		mxPush(mxEvalErrorPrototype);
		fxNewObjectInstance(the);
		mxPullSlot(mxResult);
	}
	fx_Error_aux(the);
}

void fx_RangeError(txMachine* the)
{
	if (mxResult->kind == XS_UNDEFINED_KIND) {
		mxPush(mxRangeErrorPrototype);
		fxNewObjectInstance(the);
		mxPullSlot(mxResult);
	}
	fx_Error_aux(the);
}

void fx_ReferenceError(txMachine* the)
{
	if (mxResult->kind == XS_UNDEFINED_KIND) {
		mxPush(mxReferenceErrorPrototype);
		fxNewObjectInstance(the);
		mxPullSlot(mxResult);
	}
	fx_Error_aux(the);
}

void fx_SyntaxError(txMachine* the)
{
	if (mxResult->kind == XS_UNDEFINED_KIND) {
		mxPush(mxSyntaxErrorPrototype);
		fxNewObjectInstance(the);
		mxPullSlot(mxResult);
	}
	fx_Error_aux(the);
}

void fx_TypeError(txMachine* the)
{
	if (mxResult->kind == XS_UNDEFINED_KIND) {
		mxPush(mxTypeErrorPrototype);
		fxNewObjectInstance(the);
		mxPullSlot(mxResult);
	}
	fx_Error_aux(the);
}

void fx_URIError(txMachine* the)
{
	if (mxResult->kind == XS_UNDEFINED_KIND) {
		mxPush(mxURIErrorPrototype);
		fxNewObjectInstance(the);
		mxPullSlot(mxResult);
	}
	fx_Error_aux(the);
}
