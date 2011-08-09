<?xml version="1.0"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

  <xsl:template match="/"> <!-- "/" means the whole document (root) -->
    <html>
      <head>
	<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
	<title>SCRIBO</title>
	<style type="text/css">
     	  .line
	  {
	  position:absolute;
	  background-color:white;
	  z-index:7;
	  display:inline;
	  white-space:pre;

	  /* FIXME */
	  letter-spacing:-2px;

	  padding:0px;
	  margin:0px;
	  filter:alpha(opacity=100);
	  font-family:"Times New Roman", Times, serif;
	  }
	  .para
	  {
	  position:absolute;
	  z-index:6;
	  }
	  .region
	  {
	  position:absolute;
	  z-index:5;
	  }
	  .image
	  {
	  position:absolute;
	  border:0;
	  }
	</style>
	<!--	<link rel="stylesheet" type="text/css" href="css.css" /> -->
      </head>
      <body>

	<xsl:apply-templates select="PcGts/Page/TextRegion"/>
	<xsl:apply-templates select="PcGts/Page/GraphicRegion|
				     PcGts/Page/ImageRegion|
				     PcGts/Page/ChartRegion|
				     PcGts/Page/TableRegion|
				     PcGts/Page/SeparatorRegion"/>
      </body>
    </html>
  </xsl:template>



  <!--
  TEXT REGION
  ===========
  -->

  <xsl:template match="PcGts/Page/TextRegion">

    <!-- Regions Coordinates -->

    <!-- y1 -->
    <xsl:variable name="y1">
      <xsl:for-each select="Coords/Point">
	<xsl:sort select="@y" order="ascending" data-type="number"/>
	<xsl:if test="position() = 1">
	  <xsl:value-of select="@y" />
	</xsl:if>
      </xsl:for-each>
    </xsl:variable>

    <!-- x2 -->
    <xsl:variable name="x2">
      <xsl:for-each select="Coords/Point">
	<xsl:sort select="@x" order="descending" data-type="number"/>
	<xsl:if test="position() = 1">
	  <xsl:value-of select="@x" />
	</xsl:if>
      </xsl:for-each>
    </xsl:variable>

    <!-- y2 -->
    <xsl:variable name="y2">
      <xsl:for-each select="Coords/Point">
	<xsl:sort select="@y" order="descending" data-type="number"/>
	<xsl:if test="position() = 1">
	  <xsl:value-of select="@y" />
	</xsl:if>
      </xsl:for-each>
    </xsl:variable>

    <!-- x1 -->
    <xsl:variable name="x1">
      <xsl:for-each select="Coords/Point">
	<xsl:sort select="@x" order="ascending" data-type="number"/>
	<xsl:if test="position() = 1">
	  <xsl:value-of select="@x" />
	</xsl:if>
      </xsl:for-each>
    </xsl:variable>

    <!-- END OF Regions Coordinates -->

    <xsl:apply-templates select="Line"/>

  </xsl:template>




  <!--
  TEXT LINES
  ==========
  -->
  <xsl:template match="Line">

    <!-- Regions Coordinates -->

    <!-- y1 -->
    <xsl:variable name="y1">
      <xsl:for-each select="Coords/Point">
	<xsl:sort select="@y" order="ascending" data-type="number"/>
	<xsl:if test="position() = 1">
	  <xsl:value-of select="@y" />
	</xsl:if>
      </xsl:for-each>
    </xsl:variable>

    <!-- x2 -->
    <xsl:variable name="x2">
      <xsl:for-each select="Coords/Point">
	<xsl:sort select="@x" order="descending" data-type="number"/>
	<xsl:if test="position() = 1">
	  <xsl:value-of select="@x" />
	</xsl:if>
      </xsl:for-each>
    </xsl:variable>

    <!-- y2 -->
    <xsl:variable name="y2">
      <xsl:for-each select="Coords/Point">
	<xsl:sort select="@y" order="descending" data-type="number"/>
	<xsl:if test="position() = 1">
	  <xsl:value-of select="@y" />
	</xsl:if>
      </xsl:for-each>
    </xsl:variable>

    <!-- x1 -->
    <xsl:variable name="x1">
      <xsl:for-each select="Coords/Point">
	<xsl:sort select="@x" order="ascending" data-type="number"/>
	<xsl:if test="position() = 1">
	  <xsl:value-of select="@x" />
	</xsl:if>
      </xsl:for-each>
    </xsl:variable>

    <!-- END OF Regions Coordinates -->

    <!-- TEXT LINE-->
    <xsl:variable name="color">
      <xsl:choose>
	<xsl:when test="@color != ''">
	  <xsl:value-of select="@color" />
	</xsl:when>
	<xsl:otherwise>
	  Black
	</xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

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

    <span class="line">
      <xsl:attribute name="style">
	height:auto;
	font-size:<xsl:value-of select="$a + $d" />px;
	width:<xsl:value-of select="$x2 - $x1" />px;
	left:<xsl:value-of select="$x1 " />px;
	top:<xsl:value-of select="$y1 " />px;
	color:<xsl:value-of select="$color" />;
      </xsl:attribute>
      <xsl:value-of select="@text"/>
    </span>
    <!-- ENF OF TEXT LINE -->
  </xsl:template>




  <!--
  MISC. REGIONS
  =============
  -->
  <xsl:template match="PcGts/Page/GraphicRegion|
		       PcGts/Page/ImageRegion|
		       PcGts/Page/ChartRegion|
		       PcGts/Page/TableRegion|
		       PcGts/Page/SeparatorRegion">

    <!-- Regions Coordinates -->

    <!-- y1 -->
    <xsl:variable name="y1">
      <xsl:for-each select="Coords/Point">
	<xsl:sort select="@y" order="ascending" data-type="number"/>
	<xsl:if test="position() = 1">
	  <xsl:value-of select="@y" />
	</xsl:if>
      </xsl:for-each>
    </xsl:variable>

    <!-- x2 -->
    <xsl:variable name="x2">
      <xsl:for-each select="Coords/Point">
	<xsl:sort select="@x" order="descending" data-type="number"/>
	<xsl:if test="position() = 1">
	  <xsl:value-of select="@x" />
	</xsl:if>
      </xsl:for-each>
    </xsl:variable>

    <!-- y2 -->
    <xsl:variable name="y2">
      <xsl:for-each select="Coords/Point">
	<xsl:sort select="@y" order="descending" data-type="number"/>
	<xsl:if test="position() = 1">
	  <xsl:value-of select="@y" />
	</xsl:if>
      </xsl:for-each>
    </xsl:variable>

    <!-- x1 -->
    <xsl:variable name="x1">
      <xsl:for-each select="Coords/Point">
	<xsl:sort select="@x" order="ascending" data-type="number"/>
	<xsl:if test="position() = 1">
	  <xsl:value-of select="@x" />
	</xsl:if>
      </xsl:for-each>
    </xsl:variable>

    <!-- END OF Regions Coordinates -->

    <!-- TEXT LINE-->
    <xsl:if test="name() = 'Line'">

      <xsl:variable name="color">
	<xsl:choose>
	  <xsl:when test="@color != ''">
	    <xsl:value-of select="@color" />
	  </xsl:when>
	  <xsl:otherwise>
	    Black
	  </xsl:otherwise>
	</xsl:choose>
      </xsl:variable>

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

      <div class="line" onmouseover="this.style.opacity=0.2;this.filters.alpha.opacity=20"
	   onmouseout="this.style.opacity=1;this.filters.alpha.opacity=100">
	<xsl:attribute name="style">
	  opacity:1;
	  height:auto;
	  font-size:<xsl:value-of select="$a+$d" />px;
	  width:<xsl:value-of select="$x2 - $x1" />px;
	  left:<xsl:value-of select="$x1 " />px;
	  top:<xsl:value-of select="$y1 " />px;
	  color:<xsl:value-of select="$color" />;
	</xsl:attribute>
	<xsl:value-of select="@text"/>
      </div>
    </xsl:if>
    <!-- ENF OF TEXT LINE -->

    <!-- TEXT REGION -->
    <xsl:if test="name() = 'TextRegion'">
      <div class="region">
	<xsl:attribute name="style">
	  height:<xsl:value-of select="$y2 - $y1" />px;
	  width:<xsl:value-of select="$x2 - $x1" />px;
	  left:<xsl:value-of select="$x1" />px;
	  top:<xsl:value-of select="$y1" />px;
	</xsl:attribute>
      </div>
    </xsl:if>
    <!-- ENF OF TEXT REGION -->

    <!-- PARAGRAPH -->
    <xsl:if test="name() = 'paragraph'">
      <div class="para">
	<xsl:attribute name="style">
	  height:<xsl:value-of select="$y2 - $y1" />px;
	  width:<xsl:value-of select="$x2 - $x1" />px;
	  left:<xsl:value-of select="$x1" />px;
	  top:<xsl:value-of select="$y1" />px;
	</xsl:attribute>
      </div>
    </xsl:if>
    <!-- ENF OF PARAGRAPH -->

    <!-- NON-TEXT REGIONS -->
    <xsl:if test="name() = 'ImageRegion' or name() = 'SeparatorRegion' or name() = 'GraphicRegion' or name() = 'ChartRegion' or name() = 'TableRegion'">

      <xsl:if test="container">
	<!-- data -->
	<xsl:variable name="data">
	  <xsl:value-of select="container/data" />
	</xsl:variable>

	<!-- depth -->
	<xsl:variable name="depth">
	  <xsl:choose>
	    <xsl:when test="name() = 'SeparatorRegion'">
	      1
	    </xsl:when>
	    <xsl:otherwise>
	      4
	    </xsl:otherwise>
	  </xsl:choose>
	</xsl:variable>

	<div class="image">
	  <xsl:attribute name="style">
	    top:<xsl:value-of select="$y1" />px;
	    left:<xsl:value-of select="$x1" />px;
	    width:<xsl:value-of select="$x2 - $x1"/>px;
	    height:<xsl:value-of select="$y2 - $y1"/>px;
	    z-index:<xsl:value-of select="$depth"/>;
	  </xsl:attribute>
	  <img>
	    <xsl:attribute name="alt">
	      <xsl:value-of select="name()" />
	    </xsl:attribute>
	    <xsl:attribute name="width">
	      <xsl:value-of select="$x2 - $x1" />
	    </xsl:attribute>
	    <xsl:attribute name="height">
	      <xsl:value-of select="$y2 - $y1" />
	    </xsl:attribute>
	    <xsl:attribute name="src">
	      data:image/png;base64,<xsl:value-of select="$data"/>
	    </xsl:attribute>
	  </img>
	</div>
      </xsl:if>

    </xsl:if>
    <!-- END NON-TEXT REGIONS -->
  </xsl:template>

</xsl:stylesheet>
