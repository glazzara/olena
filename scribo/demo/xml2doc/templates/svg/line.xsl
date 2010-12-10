<?xml version="1.0"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fo="http://www.w3.org/1999/XSL/Format" version="1.0">

  <!-- FILE: line.xsl
       DESCRIPTION: match all pcGts/page/text_region/paragraph/line in order to display the "text" attributes
    -->

  <xsl:template match="pcGts/page/text_region/paragraph/line">

    <!-- x_height -->
    <xsl:variable name="x_height">
      <xsl:value-of select="../../@x_height" />
    </xsl:variable>

    <!-- a_height -->
    <xsl:variable name="a_height">
      <xsl:value-of select="../../@a_height" />
    </xsl:variable>


    <!-- ABS(d_height) -->
    <xsl:variable name="d_height_abs">
      <xsl:choose>
	<xsl:when test="../../@d_height &lt; 0">
	  <xsl:value-of select="-../../@d_height" />
	</xsl:when>
	<xsl:otherwise>
	  <xsl:value-of select="../../@d_height" />
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
      <xsl:for-each select="coords/point">
	<xsl:sort select="@y" order="ascending" data-type="number"/>
	<xsl:if test="position() = 1">
	  <xsl:value-of select="@y" />
	</xsl:if>
      </xsl:for-each>
    </xsl:variable>

    <xsl:variable name="x1">
      <xsl:for-each select="coords/point">
	<xsl:sort select="@x" order="ascending" data-type="number"/>
	<xsl:if test="position() = 1">
	  <xsl:value-of select="@x" />
	</xsl:if>
      </xsl:for-each>
    </xsl:variable>
     <!-- END OF lines coordinates -->

     <!-- WTF !?, Necessary to do a lower-case ! -->
     <xsl:variable name="smallcase" select="'abcdefghijklmnopqrstuvwxyz'" />
     <xsl:variable name="uppercase" select="'ABCDEFGHIJKLMNOPQRSTUVWXYZ'" />

     <!-- Text Colour -->
     <xsl:variable name="colour">
       <xsl:choose>
	 <xsl:when test="../../@txt_colour != ''">
	   <xsl:value-of select="translate(../../@txt_colour,  $smallcase, $uppercase)" />
	 </xsl:when>
	 <xsl:otherwise>
	   black
	 </xsl:otherwise>
       </xsl:choose>
     </xsl:variable>


     <fo:block-container position="absolute">

       <xsl:attribute name="left">
	 <xsl:value-of select="$x1" />px
       </xsl:attribute>
       <xsl:attribute name="top">
	 <xsl:value-of select="$y1" />px
       </xsl:attribute>

       <xsl:attribute name="color">
	 <xsl:value-of select="$colour" />
       </xsl:attribute>

       <!-- if necessary, put letter-spacing="-Npt"  ~ -3 <= N <= -1
	    in fo:block-->

       <fo:block font-family="Times">

	 <xsl:attribute name="font-size">
	   <xsl:value-of select="$a + $d" />px
	 </xsl:attribute>

	 <xsl:value-of select="@text"/>
       </fo:block>

     </fo:block-container>

  </xsl:template>

</xsl:stylesheet>
