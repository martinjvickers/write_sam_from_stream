#include <iostream>
#include <seqan/sequence.h>  // CharString, ...
#include <seqan/stream.h>    // to stream a CharString into cout
#include <seqan/file.h>
#include <seqan/arg_parse.h>
#include <seqan/seq_io.h>
#include <seqan/bam_io.h>
using namespace seqan;
using namespace std;

/*
*/
int main(int argc, char const ** argv)
{

	CharString input = 
		"@HD	VN:1.0	SO:unsorted\n"
		"@SQ	SN:Chr1	LN:30427671\n"
		"@SQ	SN:Chr2	LN:19698289\n"
		"@SQ	SN:Chr3	LN:23459830\n"
		"@SQ	SN:Chr4	LN:18585056\n"
		"@SQ	SN:Chr5	LN:26975502\n"
		"@SQ	SN:ChrC	LN:154478\n"
		"@SQ	SN:ChrM	LN:366924\n"
		"HS3:420:C3EHMACXX:8:1101:1230:2142	0	Chr1	10962037	255	100M	*	0	0	AGTAAGAATTTAGAGAGTGATAGAGAATTTAATTGTGTATTATAGGTGGATAAAGTTATTGTGGTATTTTTTATGAGATTTAAAGATTTCGTAGATACGA	@@@DDFFDFH?DHIIIIB9EFACF@BFGGCFH<H9EFIH9FG>F*C1?D=?GGCFDFAGGFFCG=BBHEH);DD>@DCEHFGEECCDDEF;AC=@DDC1=	AS:i:-10	XN:i:0	XM:i:2	XO:i:0	XG:i:0	NM:i:2	MD:Z:89T7T2	YT:Z:UU\n"
		"HS3:420:C3EHMACXX:8:1101:1302:2100	4	*	0	0	*	*	0	0	ATTTAATGATTTTTGAAAGTTATTTGTTAATATTTTTTTTTTTTGGAAATTTGGTTTTTTTGTTTTATGATTTTTTGTATTTGTTTGATAAAAATTTGTA	?@@DDDDDHBAFHHE=@E<IHECEG<,AFHCG>FA<?F@@FHBE########################################################	YT:Z:UU\n"
		"HS3:420:C3EHMACXX:8:1101:1353:2143	4	*	0	0	*	*	0	0	TTGATTGGGTATTAAAAATATTTTTTTTTTTTTTTAATATATTTTTTTTAAAAAATCAATTTTTAAACTAAAAATTGATTTTTTTTTATTTTTTTTAAAG	@@??DD>;?AD?DGHIIICHHG@GGIGIEGEA@B##################################################################	YT:Z:UU\n"
		"HS3:420:C3EHMACXX:8:1101:1272:2165	0	Chr3	12595064	255	100M	*	0	0	ATTGTTTAGTTTTTTAATTAGATTTTTGTTTTTTTTGTATTTATATAATAAATATTTTGTGAGATTGTTTAATTAATATTTATATGAATGTTAATTTGTA	@@CDDEFDD<DFHIIGGIIHGCEGIIIEDAGGIIGIIGIG@HHIIGII@EGHGIFIH:=?CE@DDFEDCECCECD@ADCDEECCD@CDD@CDD3;@DCCD	AS:i:0	XN:i:0	XM:i:0	XO:i:0	XG:i:0	NM:i:0	MD:Z:100	YT:Z:UU\n";

	Iterator<CharString, Rooted>::Type iter = begin(input);

	StringSet<CharString> referenceNameStore;
	NameStoreCache<StringSet<CharString> > referenceNameStoreCache(referenceNameStore);
	BamIOContext<StringSet<CharString> > bamIOContext(referenceNameStore, referenceNameStoreCache);

	BamFileOut bamFileOut(toCString("meh.sam"));
	BamHeader header;
	readHeader(header, bamIOContext, iter, Sam());
	writeHeader(bamFileOut, header);

	close(bamFileOut);

	//read in the existing one
	BamFileIn bamFileIn;	
	open(bamFileIn, toCString("meh.sam"));

	//create a new one with a context copied
	BamFileOut bamFileOutAgain(context(bamFileIn), std::cout, Sam());

	String<BamAlignmentRecord> alignments;
	while (!atEnd(iter))
	{
		resize(alignments, length(alignments) + 1);
		readRecord(back(alignments), bamIOContext, iter, Sam());
	}

	for(auto& i : alignments)
        {
		writeRecord(bamFileOutAgain, i);
	}

	return 0;
}
