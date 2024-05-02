#include <pch.h>
#include "Core/Utils.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>


namespace Cober::Utils {


	bool DecomposeTransform(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale)
	{
		// From glm::decompose in matrix_decompose.inl

		using namespace glm;
		using T = float;

		mat4 LocalMatrix(transform);

		// Normalize the matrix.
		if (epsilonEqual(LocalMatrix[3][3], static_cast<float>(0), epsilon<T>()))
			return false;

		// First, isolate perspective.  This is the messiest.
		if (
			epsilonNotEqual(LocalMatrix[0][3], static_cast<T>(0), epsilon<T>()) ||
			epsilonNotEqual(LocalMatrix[1][3], static_cast<T>(0), epsilon<T>()) ||
			epsilonNotEqual(LocalMatrix[2][3], static_cast<T>(0), epsilon<T>()))
		{
			// Clear the perspective partition
			LocalMatrix[0][3] = LocalMatrix[1][3] = LocalMatrix[2][3] = static_cast<T>(0);
			LocalMatrix[3][3] = static_cast<T>(1);
		}

		// Next take care of translation (easy).
		translation = vec3(LocalMatrix[3]);
		LocalMatrix[3] = vec4(0, 0, 0, LocalMatrix[3].w);

		vec3 Row[3], Pdum3;

		// Now get scale and shear.
		for (length_t i = 0; i < 3; ++i)
			for (length_t j = 0; j < 3; ++j)
				Row[i][j] = LocalMatrix[i][j];

		// Compute X scale factor and normalize first row.
		scale.x = length(Row[0]);
		Row[0] = detail::scale(Row[0], static_cast<T>(1));
		scale.y = length(Row[1]);
		Row[1] = detail::scale(Row[1], static_cast<T>(1));
		scale.z = length(Row[2]);
		Row[2] = detail::scale(Row[2], static_cast<T>(1));

		// At this point, the matrix (in rows[]) is orthonormal.
		// Check for a coordinate system flip.  If the determinant
		// is -1, then negate the matrix and the scaling factors.

		rotation.y = asin(-Row[0][2]);
		if (cos(rotation.y) != 0) {
			rotation.x = atan2(Row[1][2], Row[2][2]);
			rotation.z = atan2(Row[0][1], Row[0][0]);
		}
		else {
			rotation.x = atan2(-Row[2][0], Row[1][1]);
			rotation.z = 0;
		}

		return true;
	}

	// Sets the String Value of a Property (for a given index)
	void DataFile::SetString(const std::string& string, const size_t item) 
    {
		if (item >= m_Content.size())
			m_Content.resize(item + 1);

		m_Content[item] = string;
	}

	// Retrieves the String Value of a Property (for a given index) or ""
	const std::string DataFile::GetString(const size_t item) const 
    {
		return item >= m_Content.size() ? "" : m_Content[item];
	}

	// Sets the Real Value of a Property (for a given index)
	void DataFile::SetReal(const double d, const size_t item) 
    {
		SetString(std::to_string(d), item);
	}

	// Retrieves the Real Value of a Property (for a given index) or 0.0
	const double DataFile::GetReal(const size_t item) const 
    {
		return std::atof(GetString(item).c_str());
	}

	void DataFile::SetVec2(const glm::vec2 vec) 
    {
		SetString(std::to_string(vec[0]), 0);
		SetString(std::to_string(vec[1]), 1);
	}

	void DataFile::SetVec3(const glm::vec3 vec) 
    {
		SetString(std::to_string(vec[0]), 0);
		SetString(std::to_string(vec[1]), 1);
		SetString(std::to_string(vec[2]), 2);
	}

	void DataFile::SetVec4(const glm::vec4 vec) 
    {
		SetString(std::to_string(vec[0]), 0);
		SetString(std::to_string(vec[1]), 1);
		SetString(std::to_string(vec[2]), 2);
		SetString(std::to_string(vec[3]), 3);
	}

	glm::vec2 DataFile::GetVec2() const 
    {
		float x = std::atof(GetString(0).c_str());
		float y = std::atof(GetString(1).c_str());
		return glm::vec2(x, y);
	}

	glm::vec3 DataFile::GetVec3() const 
    {
		float x = std::atof(GetString(0).c_str());
		float y = std::atof(GetString(1).c_str());
		float z = std::atof(GetString(2).c_str());
		return glm::vec3(x, y, z);
	}

	glm::vec4 DataFile::GetVec4() const 
    {
		float x = std::atof(GetString(0).c_str());
		float y = std::atof(GetString(1).c_str());
		float z = std::atof(GetString(2).c_str());
		float w = std::atof(GetString(3).c_str());
		return glm::vec4(x, y, z, w);
	}

	// Retrieves the Integer Value of a Property (for a given index) or 0
	void DataFile::SetInt(const int32_t number, const size_t item) 
    {
		SetString(std::to_string(number), item);
	}

	// Sets the Integer Value of a Property (for a given index)
	const int32_t DataFile::GetInt(const size_t item) const 
    {
		return std::atoi(GetString(item).c_str());
	}

	// Returns the number of Values a property consists of
	size_t DataFile::GetValueCount() const 
    {
		return m_Content.size();
	}

	// Checks if a property exists - useful to avoid creating properties via reading them, though non-essential
	bool DataFile::HasProperty(const std::string& sName) const {

		return m_MapObjects.count(sName) > 0;
	}

	// Access a datafile via a convenient name - "root.node.something.property"
	DataFile& DataFile::GetProperty(const std::string& name)
	{
		size_t x = name.find_first_of('.');
		if (x != std::string::npos)
		{
			std::string property = name.substr(0, x);
			if (HasProperty(property))
				return operator[](property).GetProperty(name.substr(x + 1, name.size()));
			else
				return operator[](property);
		}
		else
			return operator[](name);
	}


	// Access a numbered element - "node[23]", or "root[56].node"
	DataFile& DataFile::GetIndexedProperty(const std::string& name, const size_t nIndex)
	{
		return GetProperty(name + "[" + std::to_string(nIndex) + "]");
	}


	bool DataFile::Write(const DataFile& dataFile, const std::string& fileName, const std::string& indent, const char listStep) 
    {
		// Cache separator string for convenience
		std::string separator = std::string(1, listStep) + " ";

		// Cache indentation level
		size_t indentCount = 0;

		// Fully specified lambda, because this lambda is recursive!
		std::function<void(const DataFile&, std::ofstream&)> write = [&](const DataFile& dataFile, std::ofstream& file) {

			// Lambda creates string given indentation preferences
			auto indentation = [&](const std::string& string, const size_t count)
			{
				std::string out;
				for (size_t n = 0; n < count; n++)
					out += string;
				return out;
			};

			// Iterate through each property of this node
			for (auto const& property : dataFile.m_VecObjects) {

				// Does property contain any sub objects?
				if (property.second.m_VecObjects.empty()) {

					file << indentation(indent, indentCount) << property.first << (property.second.isComment ? "" : " = ");

					size_t items = property.second.GetValueCount();
					for (size_t i = 0; i < property.second.GetValueCount(); i++) {

						// If the Value being written, is string form, contains the separation
						// character, then the value must be weitten inside qutation marks.
						// Node, that if the Value is the last of a list of Values for a property,
						// it is not suffixed with the separator
						size_t x = property.second.GetString(i).find_first_of(listStep);

						if (x != std::string::npos) // Value contains separator, so wrap in quotes
							file << "\"" << property.second.GetString(i) << "\"" << ((items > 1) ? separator : "");
						else	// Value does not contain separator, so just write out
							file << property.second.GetString(i) << ((items > 1) ? separator : "");

						items--;
					}
					// Property written, move to next line
					file << "\n";
				}
				else {
					// Yes, property has properties of its own, so it's a node
					// Force a new line and write out the node's name
					file << "\n" << indentation(indent, indentCount) << property.first << "\n";
					// Open braces, and update indentation
					file << indentation(indent, indentCount) << "{\n";
					indentCount++;
					// Recursively write that node
					write(property.second, file);
					// Node written, so close braces
					file << indentation(indent, indentCount) << "}\n";
				}
			}
			// We've finished writing out a node, regardless of state, our indetation
			// must decrease, unless we're top level
			if (indentCount > 0)
				indentCount--;
		};

		// Start here!, open the file for writing
		std::ofstream file(SCENES_DIR + fileName);

		if (file.is_open())
		{
			write(dataFile, file);
			return true;
		}
		LOG_CORE_ERROR("Serializacion could'nt success with name: " + fileName);
		return false;
	}


	bool DataFile::Read(DataFile& dataFile, const std::string& fileName, const char listStep) 
    {
		std::ifstream file(SCENES_DIR + fileName);

		//Open the file!
		if (file.is_open()) {

			// These variables are outside of the read loop, as we will
			// need to refer to previous iteration values in certain conditions
			std::string propName = "";
			std::string propValue = "";

			std::stack<std::reference_wrapper<DataFile>> stackPath;
			stackPath.push(dataFile);

			while (!file.eof()) {
				// Read line
				std::string line;
				std::getline(file, line);

				// This little lambda removes whitespace from
				// beginning and end of supplied string
				auto trim = [](std::string& space) {
					space.erase(0, space.find_first_not_of(" \t\n\r\f\v"));
					space.erase(space.find_last_not_of(" \t\n\r\f\v") + 1);
				};

				trim(line);

				// If line has content
				if (!line.empty()) {

					// Test if its a comment...
					if (line[0] == '#')
					{
						// ...it is a comment, so ignore
						DataFile comment;
						comment.isComment = true;
						stackPath.top().get().m_VecObjects.push_back({ line, comment });
					}
					else {
						// ...it is content, so parse. Firstly, find if the line
						// contains an assignment. If it does then it's a property
						size_t x = line.find_first_of('=');
						if (x != std::string::npos)
						{
							// ...so split up the property into a name, and its values!

							// Extract the property name, which is all characters up to
							// first assignment, trim any whitespace from ends
							propName = line.substr(0, x);
							trim(propName);

							// Extract the property value, which is all characters after
							// the first assignment operator, trim any whitespace from ends
							propValue = line.substr(x + 1, line.size());
							trim(propValue);

							// The value may be in list form: a, b, c, d etc and some of those
							// elements mayu exist in quotes a, b, "c, d", e. So we need to iterate
							// character by character and break up the value
							bool inQuotes = false;
							std::string token;
							size_t tokenCount = 0;
							for (const auto c : propValue)
							{
								// Is character a quote...
								if (c == '\"')	// ...yes, so toggle quote state
									inQuotes = !inQuotes;
								else
								{
									// ... no, so proceed creating token. If we are in quote state
									// then just append characters until we aexit quote state.
									if (inQuotes)
										token.append(1, c);
									else {
										// Is the character our separator? If it is
										if (c == listStep) {
											// Clean up the token
											trim(token);
											// Add it to the vector of calues for this property
											stackPath.top().get()[propName].SetString(token, tokenCount);
											// Reset our token state
											token.clear();
											tokenCount++;
										}
										else  // It isnt, so just append to token
											token.append(1, c);
									}
								}
							}

							// Any rsidual characters at this point just make up the final token,
							// so clean it up and add it to the vector of values
							if (!token.empty()) {
								trim(token);
								stackPath.top().get()[propName].SetString(token, tokenCount);
							}
						}
						else // No '='
						{
							// ...but if it doesnt, then it's something structural
							if (line[0] == '{') {
								// Open brace, so push this node to stack, subsequent properties
								// wil belong to the new node
								stackPath.push(stackPath.top().get()[propName]);
							}
							else
							{
								if (line[0] == '}')	// Close brace, so this node has been defined, pop it from the stack
									stackPath.pop();
								else
								{
									// Line is a property with no assignment. Who knows whether this is useful
									// but we can simply add it as a valueless property...
									propName = line;
									// ...actually it is useful, as valuless properties are typically
									// going to be the names of the new dataFile nodes on the next iteration
								}
							}
						}
					}
				}
			}

			//Close and exist!
			file.close();
			return true;
		}

		LOG_CORE_ERROR("File not found with name: " + fileName);
		return false;
	}

}