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

    int rc; // return code


    sscanf(row,"%d %d %d %lf %lf %lf %lf %lf \n",
           &Year, &DayOfYear, &Hour1,
           &Solar,&Temp1, &RH1, &Wind, &Precip);

    /* Use xmlTextWriterStartElement instead of xmlTextWriterWriteFormatElement
     * here because we want to keep the element open to be able to add
     * attributes and child elements to it. */
    rc = xmlTextWriterStartElement(writer, BAD_CAST "observation");
    if (rc < 0) {
        printf
            ("addObservationElement: Error at xmlTextWriterStartElement");
        return;
    }

    /* Add time-of-observation attributes. */
    rc = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "year", "%d", Year);
    if (rc < 0) {
        printf
            ("addObservationElement: xmlTextWriterWriteFormatAttribute\n");
        return;
    }
    rc = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "day-of-year", "%d", DayOfYear);
    if (rc < 0) {
        printf
            ("addObservationElement: xmlTextWriterWriteFormatAttribute\n");
        return;
    }
    rc = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "hour", "%d", Hour1);
    if (rc < 0) {
        printf
            ("addObservationElement: xmlTextWriterWriteFormatAttribute\n");
        return;
    }

    /* Add the child elements, which contain the real data. */
    rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "solar", "%lf", Solar);
    if (rc < 0) {
        printf
            ("addObservationElement: Error at xmlTextWriterWriteFormatElement\n");
        return;
    }

    /* Use xmlTextWriterStartElement instead of xmlTextWriterWriteFormatElement
     * here because we want to keep the element open to be able to add an
     * attribute to it. */
    rc = xmlTextWriterStartElement(writer, BAD_CAST "temp");
    if (rc < 0) {
        printf
            ("addObservationElement: Error at xmlTextWriterStartElement\n");
        return;
    }
    /* Probably ALL data should have units indications.  Note this example uses
     * a "special character"--the degree symbol. */
    rc = xmlTextWriterWriteFormatAttribute(writer, BAD_CAST "units", "%s", "°C");
    if (rc < 0) {
        printf
            ("addObservationElement: xmlTextWriterWriteFormatAttribute\n");
        return;
    }
    rc = xmlTextWriterWriteFormatString(writer, "%lf", Temp1);
    if (rc < 0) {
        printf
            ("addObservationElement: xmlTextWriterWriteFormatString\n");
        return;
    }
    rc = xmlTextWriterEndElement(writer); /* End of "temp" element. */
    if (rc < 0) {
        printf
            ("addObservationElement: Error at xmlTextWriterEndElement\n");
        return;
    }

    /* Since this element does not have any attributes or child elements, we can
     * use the convenience "xmlTextWriterWriteFormatElement" function instead of
     * calling
     * xmlTextWriterStartElement---xmlTextWriterWriteFormatString---xmlTextWriterEndElement. */
    rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "rh", "%lf", RH1);
    if (rc < 0) {
        printf
            ("addObservationElement: Error at xmlTextWriterWriteFormatElement\n");
        return;
    }

    rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "wind", "%lf", Wind);
    if (rc < 0) {
        printf
            ("addObservationElement: Error at xmlTextWriterWriteFormatElement\n");
        return;
    }

    rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "precipitation", "%lf", Precip);
    if (rc < 0) {
        printf
            ("addObservationElement: Error at xmlTextWriterWriteFormatElement\n");
        return;
    }

    /* Done writing child elements of "observation", so close it. */
    rc = xmlTextWriterEndElement(writer);
    if (rc < 0) {
        printf
            ("addObservationElement: Error at xmlTextWriterEndElement\n");
        return;
    }

}




/* Given file handle f_h to a file of data in tab-separated-values format,
 * convert it to XML and write the resulting document to the location specified
 * by uri. */
void convertToXML(FILE *f_h, const char* uri)
{
    int rc; // return code
    xmlTextWriterPtr writer;
    char row[MAX_LINE_SIZE];

    writer = xmlNewTextWriterFilename(uri, 0);
    xmlTextWriterSetIndent(writer, 1);


    if (writer == NULL) {
        printf("convertToXML: Error creating xml writer\n");
        return;
    }

    rc = xmlTextWriterStartDocument(writer, NULL, ENCODING, NULL);
    if (rc < 0) {
        printf
            ("convertToXML: Error at xmlTextWriterStartDocument\n");
        return;
    }

    rc = xmlTextWriterStartElement(writer, BAD_CAST "data-set");
    if (rc < 0) {
        printf
            ("convertToXML: Error at xmlTextWriterStartElement\n");
        return;
    }


    while (  fgets(row, MAX_LINE_SIZE, f_h)  ) {

        addObservationElement(writer, row);

    }

    rc = xmlTextWriterEndDocument(writer);
    if (rc < 0) {
        printf("convertToXML: Error at xmlTextWriterEndDocument\n");
        return;
    }

    xmlFreeTextWriter(writer);


}

int
main(int argc, char **argv)
{
    char input_file[MAX_FILENAME_LENGTH];
    char output_file[MAX_FILENAME_LENGTH];

    FILE *f_h;
	if ((argc == 2) &&
        ( strcmp("--help", argv[1]) == 0 || strcmp("-h", argv[1]) == 0) )
    {
		printf("\nUsage: %s [input_file [output_file]]\n"
               "    output_file defaults to \"output.xml\"\n"
               "    input_file defaults to STDIN\n\n", argv[0]);
		return(0);
    }

    if (argc <= 2) {
        strcpy(output_file, "output.xml");
    }
    else {
        strcpy(output_file, argv[2]);
    }

    if (argc <= 1) {
        f_h = stdin;
    }
    else {
        strcpy(input_file, argv[1]);
        f_h = fopen(input_file, "r");
        if (f_h == NULL) {
            printf("Couldn't open %s\n", input_file);
            return 1;
        }
    }
    

    convertToXML(f_h, output_file);
}
