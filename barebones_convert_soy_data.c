#include <stdio.h>
#include <string.h>
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>

const char ENCODING[] = "UTF-8"; /* The encoding we want for our XML document. */
const int MAX_LINE_SIZE = 200;   /* The maximum line length for the input file. */
const int MAX_FILENAME_LENGTH = 200;

/* Uses the data in "row" to add a child "observation" element to the
 * currently-open element. */
void
addObservationElement(xmlTextWriterPtr writer, char *row)
{
    int Year;
    int DayOfYear;
    int Hour1;

    double Solar;
    double Temp1;
    double RH1;
    double Wind;
    double Precip;


    sscanf(row,"%d %d %d %lf %lf %lf %lf %lf \n",
           &Year, &DayOfYear, &Hour1,
           &Solar,&Temp1, &RH1, &Wind, &Precip);



    
    xmlTextWriterStartElement(writer, BAD_CAST "observation");

    
    /* Add time-of-observation attributes. */
    xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "year", "%d", Year);
    xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "day-of-year", "%d", DayOfYear);
    xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "hour", "%d", Hour1);

    /* Add the child elements, which contain the real data. */
    xmlTextWriterWriteFormatElement(writer, BAD_CAST "solar", "%lf", Solar);

    xmlTextWriterStartElement(writer, BAD_CAST "temp");
    xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "units", "%s", "°C");
    xmlTextWriterWriteFormatString(writer, "%lf", Temp1);
    xmlTextWriterEndElement(writer); /* End of "temp" element. */

    xmlTextWriterWriteFormatElement(writer, BAD_CAST "rh", "%lf", RH1);
    xmlTextWriterWriteFormatElement(writer, BAD_CAST "wind", "%lf", Wind);
    xmlTextWriterWriteFormatElement(writer, BAD_CAST "precipitation", "%lf", Precip);


    xmlTextWriterEndElement(writer);

}


/* Given file handle f_h to a file of data in tab-separated-values format,
 * convert it to XML and write the resulting document to the location specified
 * by uri. */
void convertToXML(FILE *f_h, const char* uri)
{
    xmlTextWriterPtr writer;
    char row[MAX_LINE_SIZE];

    writer = xmlNewTextWriterFilename(uri, 0);
    xmlTextWriterSetIndent(writer, 1);

    
    xmlTextWriterStartDocument(writer, NULL, ENCODING, NULL);

    xmlTextWriterStartElement(writer, BAD_CAST "data-set");

    while (  fgets(row, MAX_LINE_SIZE, f_h)  ) {
        addObservationElement(writer, row);
    }

    xmlTextWriterEndDocument(writer);

    xmlFreeTextWriter(writer);
}

int
main(int argc, char **argv)
{
    FILE *f_h;

    f_h = fopen("2002SoyData", "r");
    convertToXML(f_h, "output.xml");
}
