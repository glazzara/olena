<?xml version="1.0"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fo="http://www.w3.org/1999/XSL/Format" version="1.0">

  <!-- FILE: regions.xsl
       DESCRIPTION: Match all regions that are not text to display them -->

  <xsl:template match="pcGts/page/image_region|
		       pcGts/page/graphic_region|
		       pcGts/page/chart_region|
		       pcGts/page/table_region|
		       pcGts/page/separator_region">

    <!-- ID of the region, used to display id.png -->
    <xsl:variable name="data">
      <xsl:value-of select="container/data" />
    </xsl:variable>

    <!-- Regions coordinates -->
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

    <xsl:variable name="y2">
      <xsl:for-each select="coords/point">
	<xsl:sort select="@y" order="descending" data-type="number"/>
	<xsl:if test="position() = 1">
	  <xsl:value-of select="@y" />
	</xsl:if>
      </xsl:for-each>
    </xsl:variable>

    <xsl:variable name="x2">
      <xsl:for-each select="coords/point">
	<xsl:sort select="@x" order="descending" data-type="number"/>
	<xsl:if test="position() = 1">
	  <xsl:value-of select="@x" />
	</xsl:if>
      </xsl:for-each>
    </xsl:variable>
    <!-- END OF regions coordinates -->

    <fo:block-container>

      <fo:block>
	<fo:instream-foreign-object>
	  <svg:svg xmlns:svg="http://www.w3.org/2000/svg"
		   xmlns:xlink="http://www.w3.org/1999/xlink"
		   Version="1.1">

	    <xsl:attribute name="viewBox">
	      <xsl:value-of select="$x1" /> <xsl:value-of select="$y1" /> <xsl:value-of select="$x2" /> <xsl:value-of select="$y2" />
	    </xsl:attribute>

            <svg:image position="absolute">

	      <xsl:attribute name="y">
		<xsl:value-of select="$y1" />
	      </xsl:attribute>

	      <xsl:attribute name="x">
		<xsl:value-of select="$x1" />
	      </xsl:attribute>

	      <xsl:attribute name="width">
		<xsl:value-of select="$x2 - $x1" />
	      </xsl:attribute>

	      <xsl:attribute name="height">
		<xsl:value-of select="$y2 - $y1" />
	      </xsl:attribute>

              <xsl:attribute name="xlink:href">data:image/png;base64,<xsl:value-of select="$data" /></xsl:attribute>
	    </svg:image>

	  </svg:svg>

	</fo:instream-foreign-object>

      </fo:block>
    </fo:block-container>

  </xsl:template>

</xsl:stylesheet>
