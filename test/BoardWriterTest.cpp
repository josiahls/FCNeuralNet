/**
 * FCNeuralNet
 * Created by Laivins, Josiah https://josiahls.github.io/ on 2019-03-08
 *
 * This software is provided 'as-is', without any express or implied warranty.
 *
 * In no event will the author(s) be held liable for any damages arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it freely,
 * subject to the following restrictions:
 * 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */


#include <gtest/gtest.h>
#include "../src/utils/BoardWriter.h"

TEST (BoardWriter, BoardWriter_Log_Dir_Test) {
    BoardWriter writer(std::string(), std::string(), false);
    writer.write("Accuracy", 0.3, BoardWriter::ONE_D_VECTOR);
    ASSERT_TRUE(exists(writer.logBaseFilePath + "_Accuracy.csv"));
    writer.removeSubDir();
    ASSERT_TRUE(!exists(writer.logBaseFilePath));
}

TEST (BoardWriter, BoardWriter_Log_ReadWrite_Test) {
    BoardWriter writer(std::string(), std::string(), false);
    ASSERT_TRUE(!exists(writer.logBaseFilePath));
    writer.write("Accuracy", 0.3, BoardWriter::ONE_D_VECTOR);
    ASSERT_TRUE(exists(writer.logBaseFilePath + "_Accuracy.csv"));
    writer.write("Accuracy", 3.1, BoardWriter::ONE_D_VECTOR);
    writer.write("Accuracy", 5.2, BoardWriter::ONE_D_VECTOR);
    writer.write("Accuracy", 7.6, BoardWriter::ONE_D_VECTOR);
    writer.write("Accuracy", 4.8, BoardWriter::ONE_D_VECTOR);

    writer.write("Training", 7.2, BoardWriter::ONE_D_VECTOR);
    ASSERT_TRUE(exists(writer.logBaseFilePath + "_Training.csv"));
    writer.write("Training", 4.8, BoardWriter::ONE_D_VECTOR);
    writer.write("Training", 9.8, BoardWriter::ONE_D_VECTOR);
    writer.write("Training", 65, BoardWriter::ONE_D_VECTOR);

    writer.removeSubDir();
    ASSERT_TRUE(!exists(writer.logBaseFilePath));
}