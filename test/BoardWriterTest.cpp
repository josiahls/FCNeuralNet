//
// Created by Laivins, Josiah on 2019-03-15.
//

#include <gtest/gtest.h>
#include "../src/utils/BoardWriter.h"

TEST (BoardWriter, BoardWriter_Log_Dir_Test) {
    BoardWriter writer;
    writer.write("Accuracy", 0.3, BoardWriter::ONE_D_VECTOR);
    ASSERT_TRUE(exists(writer.logBaseFilePath + "_Accuracy.csv"));
    writer.removeSubDir();
    ASSERT_TRUE(!exists(writer.logBaseFilePath));
}

TEST (BoardWriter, BoardWriter_Log_ReadWrite_Test) {
    BoardWriter writer;
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