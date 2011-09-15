<?xml version="1.0"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fo="http://www.w3.org/1999/XSL/Format" version="1.0">

  <!-- FILE: line.xsl
       DESCRIPTION: match all PcGts/Page/TextRegion/Line in order to display the "text" attributes
  -->

  <xsl:template match="PcGts/Page/TextRegion">

      <!-- WTF !?, Necessary to do a lower-case !
	   FIXME: take a look at text-transform attribute -->
      <xsl:variable name="smallcase" select="'abcdefghijklmnopqrstuvwxyz'" />
      <xsl:variable name="uppercase" select="'ABCDEFGHIJKLMNOPQRSTUVWXYZ'" />

      <!-- Text Colour -->
      <xsl:variable name="colour">
	<xsl:choose>
	  <xsl:when test="@color != ''">
	    <xsl:value-of select="translate(@color,  $smallcase, $uppercase)" />
	  </xsl:when>
	  <xsl:otherwise>
	    black
	  </xsl:otherwise>
	</xsl:choose>
      </xsl:variable>


    <!-- char_width -->
    <xsl:variable name="char_width">
      <xsl:value-of select="@charWidth" />
    </xsl:variable>


    <xsl:for-each select="Line">

      <!-- x_height -->
      <xsl:variable name="x_height">
	<xsl:value-of select="@xHeight" />
      </xsl:variable>

      <!-- a_height -->
      <xsl:variable name="a_height">
	<xsl:value-of select="@aHeight" />
      </xsl:variable>


      <!-- ABS(d_height) -->
      <xsl:variable name="d_height_abs">
	<xsl:choose>
	  <xsl:when test="@dHeight &lt; 0">
	    <xsl:value-of select="-@dHeight" />
	  </xsl:when>
	  <xsl:otherwise>
	    <xsl:value-of select="@dHeight" />
	  </xsl:otherwise>
	</xsl:choose>
      </xsl:variable>

      <!-- a_height computed, i.e>
	   if (A - X < |D|)
	   A = X + |D|
      -->
      <xsl:variable name="a">
	<xsl:choose>
	  <xsl:when test="($a_height - $x_height) &lt; $d_height_abs">
	    <xsl:value-of select="$x_height + $d_height_abs" />
	  </xsl:when>
	  <xsl:otherwise>
	    <xsl:value-of select="$a_height" />
	  </xsl:otherwise>
	</xsl:choose>
      </xsl:variable>

      <!-- d_height computed, i.e>
	   if (A - A > |D|)
	   |D| = A - X
      -->
      <xsl:variable name="d">
	<xsl:choose>
	  <xsl:when test="($a_height - $x_height) &gt; $d_height_abs">
	    <xsl:value-of select="$a_height - $x_height" />
	  </xsl:when>
	  <xsl:otherwise>
	    <xsl:value-of select="$d_height_abs" />
	  </xsl:otherwise>
	</xsl:choose>
      </xsl:variable>

      <!-- Text lines coordinates -->
      <xsl:variable name="y1">
	<xsl:for-each select="Coords/Point">
	  <xsl:sort select="@y" order="ascending" data-type="number"/>
	  <xsl:if test="position() = 1">
	    <xsl:value-of select="@y" />
	  </xsl:if>
	</xsl:for-each>
      </xsl:variable>

      <xsl:variable name="ymax">
	<xsl:for-each select="Coords/Point">
	  <xsl:sort select="@y" order="ascending" data-type="number"/>
	  <xsl:if test="position() = 3">
	    <xsl:value-of select="@y" />
	  </xsl:if>
	</xsl:for-each>
      </xsl:variable>

      <xsl:variable name="x1">
	<xsl:for-each select="Coords/Point">
	  <xsl:sort select="@x" order="ascending" data-type="number"/>
	  <xsl:if test="position() = 1">
	    <xsl:value-of select="@x" />
	  </xsl:if>
	</xsl:for-each>
      </xsl:variable>

      <xsl:variable name="xmax">
	<xsl:for-each select="Coords/Point">
	  <xsl:sort select="@x" order="ascending" data-type="number"/>
	  <xsl:if test="position() = 3">
	    <xsl:value-of select="@x" />
	  </xsl:if>
	</xsl:for-each>
      </xsl:variable>
      <!-- END OF lines coordinates -->

      <fo:block-container position="absolute" border-width="5mm">

	<xsl:attribute name="left">
	  <xsl:value-of select="$x1" />px
	</xsl:attribute>
	<xsl:attribute name="top">
	  <xsl:value-of select="$y1" />px
	</xsl:attribute>

	<xsl:attribute name="right">
	  <xsl:value-of select="$xmax" />px
	</xsl:attribute>
	<xsl:attribute name="bottom">
	  <xsl:value-of select="$ymax" />px
	</xsl:attribute>


	<xsl:attribute name="width">
	  <xsl:value-of select="$xmax - $x1" />px
	</xsl:attribute>

	<xsl:attribute name="color">
	  <xsl:value-of select="$colour" />
	</xsl:attribute>

	<!-- Adjusting height if font is different from Times. -->
	<xsl:variable name="fsize">
	  <xsl:choose>
	    <xsl:when test="($a + $d) &gt; (1.16 * (37 * $char_width) div 17)">
	      <xsl:value-of select="((37 * $char_width) div 17)" />
	    </xsl:when>
	    <xsl:otherwise>
	      <xsl:value-of select="($a + $d)" />
	    </xsl:otherwise>
	  </xsl:choose>
	</xsl:variable>

	<!-- if necessary, put letter-spacing="-Npt"  ~ -3 <= N <= -1
	     in fo:block-->

	<!-- text-align-last="justify" will help justifying and using a
	     uniform font size (it stretchs the text, at least for PDF)
	     but it relies on several lines... We need paragraph
	     information.
	-->

	<!-- FIXME: using a table allows to justify a single line of
	     text. This is an UGLY HACK. Font size is also tweaked for
	     now but it should not since we have font information.
	-->
	<fo:table table-layout="fixed">

	  <xsl:attribute name="width">
	    <xsl:value-of select="$xmax -$x1" />px
	  </xsl:attribute>

	  <fo:table-column  column-number="1">
	    <xsl:attribute name="column-width">
	      <xsl:value-of select="$xmax -$x1" />px
	    </xsl:attribute>
	  </fo:table-column>

	  <fo:table-body start-indent="0pt" text-align="justify" text-align-last="justify">

	    <fo:table-row>

	      <fo:table-cell>

		<fo:block font-family="Times" wrap-option="no-wrap" white-space-collapse="true" text-align-last="justify" text-align="justify">

		  <xsl:attribute name="font-size">
		    <xsl:value-of select="$fsize" />px
		  </xsl:attribute>

		  <xsl:value-of select="@text"/>
		</fo:block>

	      </fo:table-cell>

	    </fo:table-row>

	  </fo:table-body>

	</fo:table>


      </fo:block-container>

    </xsl:for-each>

  </xsl:template>

</xsl:stylesheet>
