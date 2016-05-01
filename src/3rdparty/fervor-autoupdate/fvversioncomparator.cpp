/*
 * This file is part of QRK - Qt Registrier Kasse
 *
 * Copyright (C) 2015-2016 Christian Kvasny <chris@ckvsoft.at>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 * Button Design, and Idea for the Layout are lean out from LillePOS, Copyright 2010, Martin Koller, kollix@aon.at
 *
*/

#include "fvversioncomparator.h"
#include <string>
#include <algorithm>
#include <cctype>

//
// Clone of Sparkle's SUStandardVersionComparator.m, so here's original author's
// copyright too:
//
//	Copyright 2007 Andy Matuschak. All rights reserved.
//
// Everything's the same except for TypeOfCharacter()
// (because who knows how Foundation does isdigit() and such.)
//


FvVersionComparator::FvVersionComparator()
{
	// noop
}

FvVersionComparator::CharacterType FvVersionComparator::TypeOfCharacter(std::string character)
{
	if (character == ".") {
		return kSeparatorType;
	} else if (isdigit(character[0])) {
		return kNumberType;
	} else if (isspace(character[0])) {
		return kSeparatorType;
	} else if (ispunct(character[0])) {
		return kSeparatorType;
	} else {
		return kStringType;
	}

}

std::vector<std::string> FvVersionComparator::SplitVersionString(std::string version)
{
	std::string character;
	std::string s;
	unsigned long i = 0, n = 0;
	CharacterType oldType, newType;
	std::vector<std::string> parts;

	if (version.length() == 0) {
		// Nothing to do here
		return parts;
	}

	s = version.substr(0, 1);
	oldType = TypeOfCharacter(s);
	n = version.length() - 1;
	for (i = 1; i <= n; ++i) {
		character = version.substr(i, 1)[0];
		newType = TypeOfCharacter(character);
		if (oldType != newType || oldType == kSeparatorType) {
			// We've reached a new segment
			std::string aPart = s;
			parts.push_back(aPart);
			s = character;
		} else {
			// Add character to string and continue
			s.append(character);
		}
		oldType = newType;
	}

	// Add the last part onto the array
	parts.push_back(s);
	return parts;
}


FvVersionComparator::ComparatorResult FvVersionComparator::CompareVersions(std::string versionA,
																		   std::string versionB)
{
	std::vector<std::string> partsA = SplitVersionString(versionA);
	std::vector<std::string> partsB = SplitVersionString(versionB);

	std::string partA = std::string(""), partB = std::string("");
	unsigned long i = 0, n = 0;
	int intA, intB;
	CharacterType typeA, typeB;

	n = std::min(partsA.size(), partsB.size());
	for (i = 0; i < n; ++i) {
		partA = partsA.at(i);
		partB = partsB.at(i);

		typeA = TypeOfCharacter(partA);
		typeB = TypeOfCharacter(partB);

		// Compare types
		if (typeA == typeB) {
			// Same type; we can compare
			if (typeA == kNumberType) {
				intA = atoi(partA.c_str());
				intB = atoi(partB.c_str());

				if (intA > intB) {
					return kDescending;
				} else if (intA < intB) {
					return kAscending;
				}
			} else if (typeA == kStringType) {
				short result = partA.compare(partB);
				switch (result) {
					case -1: return kAscending; break;
					case 1: return kDescending; break;
					case 0: /* do nothing */ break;
				};
			}
		} else {
			// Not the same type? Now we have to do some validity checking
			if (typeA != kStringType && typeB == kStringType) {
				// typeA wins
				return kDescending;
			} else if (typeA == kStringType && typeB != kStringType) {
				// typeB wins
				return kAscending;
			} else {
				// One is a number and the other is a period. The period is invalid
				if (typeA == kNumberType) {
					return kDescending;
				} else {
					return kAscending;
				}
			}
		}
	}
	// The versions are equal up to the point where they both still have parts
	// Lets check to see if one is larger than the other
	if (partsA.size() != partsB.size()) {
		// Yep. Lets get the next part of the larger
		// n holds the index of the part we want.
		std::string missingPart = std::string("");
		CharacterType missingType;
		ComparatorResult shorterResult, largerResult;

		if (partsA.size() > partsB.size()) {
			missingPart = partsA.at(n);
			shorterResult = kAscending;
			largerResult = kDescending;
		} else {
			missingPart = partsB.at(n);
			shorterResult = kDescending;
			largerResult = kAscending;
		}

		missingType = TypeOfCharacter(missingPart);
		// Check the type
		if (missingType == kStringType) {
			// It's a string. Shorter version wins
			return shorterResult;
		} else {
			// It's a number/period. Larger version wins
			return largerResult;
		}
	}

	// The 2 strings are identical
	return kSame;
}
