TEMPLATE = subdirs

SUBDIRS += \
    WriterDL \
    WriterTest \
    WriterLib

WriterDL.depends = WriterLib
WriterTest.depends = WriterLib
